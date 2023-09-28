/*
 * CApp.cpp
 *
 *  Created on: 13 sept. 2021
 *      Author: eleve
 */

#include <iostream>
#include <cstdio>
#include <ctime>

using namespace std;

#include <macros.h>
#include <parameters.h>

#include "CApp.h"

enum e_status_flags{
	ST_ALL_CLEARED		= 0x00000000,
	ST_SDL_INITIATED	= 0x00000001,
	ST_TTF_INITIATED	= 0x00000002,
	ST_APP_INIT_FAILED	= 0x80000000,
	ST_APP_TITLE_SCREEN	= 0x00000010,
	ST_APP_PAUSED		= 0x00000100,
	ST_APP_GAMEOVER		= 0x00001000,
};

Uint32 CApp::_AppTimerTimeCallback(Uint32 interval, CApp* pApp){
	if (mIsBitsSet(pApp->m_uStatus, ST_APP_PAUSED) || mIsBitsSet(pApp->m_uStatus, ST_APP_GAMEOVER))
		return interval;

	pApp->m_iTime += interval/1000;
	char buf[32];
	sprintf(buf,"%04d",pApp->m_iTime);

	pApp->m_pTimerTexts->Pushback(new CText(pApp->m_pRenderer, SDL_Point{APP_WINDOW_PADDING+(SNAKE_POS_MAX_X)*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)-100,5}, buf));

	while(pApp->m_pTimerTexts->Card()>3 && pApp->m_pScoreTexts->Card()>3){
		delete ((CText*)pApp->m_pScoreTexts->Popfront());
		delete ((CText*)pApp->m_pTimerTexts->Popfront());
	}

	return interval;
}


void CApp::DrawAndAnimate(){

	if(mIsBitsSet(m_uStatus, ST_APP_PAUSED) || mIsBitsSet(m_uStatus, ST_APP_GAMEOVER)){
		m_pSound->MusicPause();
		return;
	}

	static SDL_Rect rGameArea = {
			APP_WINDOW_PADDING-1,
			APP_WINDOW_PADDING-1,
			(SNAKE_POS_MAX_X)*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)+1,
			(SNAKE_POS_MAX_Y)*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)+1
	};
	
	// --- GAME AREA DRAWING --- // 
	SDL_RenderClear(m_pRenderer);
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, nullptr, &SDL_Rect{0,0,APP_WINDOW_WIDTH,APP_WINDOW_HEIGHT});
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(m_pRenderer, &rGameArea);

	// --- SNAKE LOGIC --- //
	if(m_pSnake)m_pSnake->Draw(m_pRenderer); //test
	if(m_pSnake)m_pSnake->Move();


	// --- FOOD LOGIC--- //
	if(!m_iSnakeFoodTimeout){
		int iPosX = 0;
		int iPosY = 0;

		do {
			iPosX = rand() % SNAKE_POS_MAX_X;
			iPosY = rand() % SNAKE_POS_MAX_Y;
		} while (m_pSnake->IsOnCell(iPosX, iPosY));

		m_ptSnakeFoodLoc = SDL_Point{iPosX, iPosY};
		m_iSnakeFoodTimeout = rand() % 20;
	}

	if (m_ptSnakeFoodLoc.x == -1) m_iSnakeFoodTimeout -= 1;
	else {

		SDL_Rect rectFood = {
			APP_WINDOW_PADDING + m_ptSnakeFoodLoc.x * (SNAKE_SECTION_SIZE + SNAKE_SECTION_SPACING) + SNAKE_FOOD_PADDING,
			APP_WINDOW_PADDING + m_ptSnakeFoodLoc.y * (SNAKE_SECTION_SIZE + SNAKE_SECTION_SPACING) + SNAKE_FOOD_PADDING,
			SNAKE_FOOD_SIZE,
			SNAKE_FOOD_SIZE
		};
		SDL_RenderCopy(m_pRenderer, m_pFoodTexture, nullptr, &rectFood);
	}

	// --- FOOD DETECTION --- //
	if (m_pSnake->IsFoodDetected(m_ptSnakeFoodLoc)){
		m_pSnake->GrowUp();
		m_pSound->CollectPlay();
		m_ptSnakeFoodLoc = SDL_Point{-1,-1};
		m_iScore += 50;
		
		char buf[64];
		sprintf(buf,"%04d",m_iScore);
		m_pScoreTexts->Pushback(new CText(m_pRenderer, SDL_Point{APP_WINDOW_PADDING, 5}, buf));
	}

	// --- POISON LOGIC --- //
	if(!m_iSnakePoisonTimeout){
		int iPosX = 0;
		int iPosY = 0;

		do {
			iPosX = rand() % SNAKE_POS_MAX_X;
			iPosY = rand() % SNAKE_POS_MAX_Y;
		} while (m_pSnake->IsOnCell(iPosX, iPosY));

		m_ptSnakePoisonLoc = SDL_Point{iPosX, iPosY};
		m_iSnakePoisonTimeout = rand() % 20;
	}

	if (m_ptSnakePoisonLoc.x==-1) m_iSnakePoisonTimeout -= 1;
	else {
		SDL_Rect rectFood = {
			APP_WINDOW_PADDING + m_ptSnakePoisonLoc.x * (SNAKE_SECTION_SIZE + SNAKE_SECTION_SPACING) + SNAKE_FOOD_PADDING,
			APP_WINDOW_PADDING + m_ptSnakePoisonLoc.y * (SNAKE_SECTION_SIZE + SNAKE_SECTION_SPACING) + SNAKE_FOOD_PADDING,
			SNAKE_FOOD_SIZE,
			SNAKE_FOOD_SIZE
		};
		SDL_RenderCopy(m_pRenderer, m_pPoisonTexture, nullptr, &rectFood);
	}

	// --- POISON DETECTION --- //
	if (m_pSnake->IsFoodDetected(m_ptSnakePoisonLoc)){
		m_pSnake->Shrink();
		m_pSound->PoisonPlay();
		m_ptSnakePoisonLoc = SDL_Point{-1,-1};
		m_iScore -= 50;

		char buf[64];
		sprintf(buf,"%04d",m_iScore);
		m_pScoreTexts->Pushback(new CText(m_pRenderer, SDL_Point{APP_WINDOW_PADDING, 5}, buf));
	}

	// --- SCORE AND TIMER RENDERING --- //
	if(m_pScoreTexts->Card())((CText*)m_pScoreTexts->Getback())->Draw(m_pRenderer);
	if(m_pTimerTexts->Card())((CText*)m_pTimerTexts->Getback())->Draw(m_pRenderer);


	// --- SELF-BITING DETECTION --- //
	if(m_pSnake->IsBiteHimself()){	
		mBitsSet(m_uStatus, ST_APP_GAMEOVER);
		SDL_RenderCopy(m_pRenderer, m_pTextGameover, nullptr, &m_rTextGameover);
	}

	// --- RENDERING --- //
	SDL_RenderPresent(m_pRenderer);
}


CApp::CApp():
	m_uStatus(ST_ALL_CLEARED),
	m_iTime(0),
	m_ptSnakeFoodLoc(SDL_Point{-1,-1}),
	m_ptSnakePoisonLoc(SDL_Point{-1,-1}),
	m_iSnakeFoodTimeout(0),
	m_iSnakePoisonTimeout(0),
	m_iScore(0){
	srand((unsigned int)time(nullptr));

	if(SDL_Init(SDL_INIT_EVERYTHING)){
		cerr << "SDL Initialization failed: " << SDL_GetError() << endl;
		mBitsSet(m_uStatus, ST_APP_INIT_FAILED);
	}

	m_pWindow = SDL_CreateWindow("The Snake Game but it's C++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT, 0);
	if(!m_pWindow){
		cerr << "SDL CreateWindow() failed: " << SDL_GetError() << endl;
		mBitsSet(m_uStatus, ST_APP_INIT_FAILED);
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);
	if(!m_pRenderer){
		cerr << "SDL Hardware CreateRenderer() failed, resorting to creating a software renderer" << endl;
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if(!m_pRenderer){
			cerr << "SDL Software CreateRenderer() failed: " << SDL_GetError() << endl;
			mBitsSet(m_uStatus, ST_APP_INIT_FAILED);
		}
	}
	mBitsSet(m_uStatus, ST_SDL_INITIATED);
	TTF_Init();
	m_pFont = TTF_OpenFont(APP_MAIN_FONT_FILE_NAME, APP_FONT_SIZE);
	if (m_pFont == nullptr) {
		cerr << "TTF failed to load font file: " <<  SDL_GetError() << endl;
		mBitsSet(m_uStatus, ST_APP_INIT_FAILED);
	}
	else
		mBitsSet(m_uStatus, ST_TTF_INITIATED);

	int iW, iH;
	SDL_Surface* pCacheSurface = TTF_RenderText_Blended(m_pFont, "Game over | Press - to restart", SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});	
	m_pTextGameover = SDL_CreateTextureFromSurface(m_pRenderer, pCacheSurface);
	SDL_FreeSurface(pCacheSurface);
	SDL_QueryTexture(m_pTextGameover, nullptr, nullptr, &iW, &iH);
	m_rTextGameover = SDL_Rect{APP_WINDOW_WIDTH/2-iW/2, 5, iW, iH};

	pCacheSurface = TTF_RenderText_Blended(m_pFont, "PAUSED", SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
	m_pTextPaused = SDL_CreateTextureFromSurface(m_pRenderer, pCacheSurface);
	SDL_FreeSurface(pCacheSurface);
	SDL_QueryTexture(m_pTextPaused, nullptr, nullptr, &iW, &iH);
	m_rTextPaused = SDL_Rect{APP_WINDOW_WIDTH/2-iW/2, 5, iW, iH};

	m_colBkgnd 		= APP_WINDOW_COLOR_BKGND;
	m_colLimits		= APP_WINDOW_COLOR_LIMITS;

	m_pFoodTexture 		= IMG_LoadTexture(m_pRenderer, APP_FOOD_APPLE_PATH);
	m_pPoisonTexture	= IMG_LoadTexture(m_pRenderer, APP_FOOD_POISON_PATH);
	m_pBGTexture		= IMG_LoadTexture(m_pRenderer, APP_DEFAULT_BG_PATH);
	m_pTitleScreen		= IMG_LoadTexture(m_pRenderer, APP_DEFAULT_TITLE_PATH);

	SDL_GameControllerOpen(0);

	CSnake::InitGraph(m_pRenderer, APP_SNAKE_HEAD_PATH, APP_SNAKE_BODY_PATH, APP_SNAKE_TAIL_PATH);
	CText::InitGraph(APP_MAIN_FONT_FILE_NAME, SDL_Color{0xFF, 0xFF, 0xFF, 0xFF}, APP_FONT_SIZE);
	
	mBitsSet(m_uStatus, ST_APP_TITLE_SCREEN);
	Run();

}

CApp::~CApp() {
	CText::ReleaseGraph();
	CSnake::ReleaseGraph();

	SDL_RemoveTimer(m_iTimerIDtimer);

	if(m_pScoreTexts)delete m_pScoreTexts;
	if(m_pTimerTexts)delete m_pTimerTexts;

	if(m_pSound)delete m_pSound;
	if(m_pSnake)delete m_pSnake;

	SDL_DestroyTexture(m_pTextPaused);
	SDL_DestroyTexture(m_pTextGameover);
	
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pTitleScreen);
	SDL_DestroyTexture(m_pPoisonTexture);
	SDL_DestroyTexture(m_pFoodTexture);

	if(m_pFont)TTF_CloseFont(m_pFont);
	if(m_pRenderer)SDL_DestroyRenderer(m_pRenderer);
	if(m_pWindow)SDL_DestroyWindow(m_pWindow);

	TTF_Quit();
	SDL_Quit();
}

int CApp::Run(){
	if(mIsBitsSet(m_uStatus, ST_APP_INIT_FAILED))
		return m_uStatus;

	SDL_Event	event;
	int 		quit = 0;

	m_pSnake		= new CSnake();
	m_pSound		= new CSound();
	m_pScoreTexts		= new CContainer((t_ptfV)CText::DeleteFunc);
	m_pTimerTexts		= new CContainer((t_ptfV)CText::DeleteFunc);

	m_pTimerTexts->Pushback(new CText(m_pRenderer, SDL_Point{APP_WINDOW_PADDING+(SNAKE_POS_MAX_X)*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)-100,5}, "0000"));
	m_pScoreTexts->Pushback(new CText(m_pRenderer, SDL_Point{APP_WINDOW_PADDING, 5}, "0000"));

	while(!quit){
		while(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_CONTROLLERBUTTONDOWN:
				switch(event.cbutton.button){
				case SDL_CONTROLLER_BUTTON_BACK:
					if (mIsBitsSet(m_uStatus, ST_APP_GAMEOVER)) {
						ResetGameState();
					}
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					m_pSnake->ChangeDirection(DIR_UP);
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					m_pSnake->ChangeDirection(DIR_DOWN);
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
					m_pSnake->ChangeDirection(DIR_LEFT);
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
					m_pSnake->ChangeDirection(DIR_RIGHT);
					break;
				case SDL_CONTROLLER_BUTTON_START:
					if (mIsBitsSet(m_uStatus, ST_APP_TITLE_SCREEN)) {
						mBitsClr(m_uStatus, ST_APP_TITLE_SCREEN);
						m_iTimerIDtimer   = SDL_AddTimer(1000, (SDL_TimerCallback)_AppTimerTimeCallback, this);
						m_pSound->MusicPlay();
						break;
					}

					else if(mIsBitsClr(m_uStatus, ST_APP_GAMEOVER) && mIsBitsClr(m_uStatus, ST_APP_PAUSED)){
						mBitsSet(m_uStatus, ST_APP_PAUSED);
						m_pSound->PausePlay();
						SDL_RenderCopy(m_pRenderer, m_pTextPaused, nullptr, &m_rTextPaused);
						SDL_RenderPresent(m_pRenderer);
					}

					else{
						m_pSound->MusicPlay();
						mBitsClr(m_uStatus, ST_APP_PAUSED);
						m_pSound->PausePlay();
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
		
		if (mIsBitsClr(m_uStatus, ST_APP_TITLE_SCREEN)) DrawAndAnimate();
		else {
			SDL_RenderCopy(m_pRenderer, m_pTitleScreen, nullptr, nullptr);
			SDL_RenderPresent(m_pRenderer);
		}
		SDL_Delay(APP_TIMER_TICK_MS);
	}
	return m_uStatus;
}

void CApp::ResetGameState() {
	/* Setting the score and timer back to zero */
	m_iScore = 0;
	m_iTime = 0;

	/* Creating a new snake object and deleting the old ones */
	CSnake* pCache = m_pSnake;
	m_pSnake = new CSnake();
	delete pCache;

	/* Resetting the food and poison positions */
	m_ptSnakeFoodLoc.x = -1;
	m_ptSnakePoisonLoc.x = -1;

	/* Pushing back new score and timer textures */
	m_pTimerTexts->Pushback(new CText(m_pRenderer, SDL_Point{APP_WINDOW_PADDING+(SNAKE_POS_MAX_X)*(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)-100,5}, "0000"));
	m_pScoreTexts->Pushback(new CText(m_pRenderer, SDL_Point{APP_WINDOW_PADDING, 5}, "0000"));

	/* Clearing the game over flag */
	mBitsClr(m_uStatus, ST_APP_GAMEOVER);
}
