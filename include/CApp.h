/*
 * CApp.h
 *
 *  Created on: 13 sept. 2021
 *      Author: eleve
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>

#include <CSnake.h>
#include <CSound.h>
#include <CContainer.h>
#include <CText.h>

class CApp {
private:
	SDL_Window*		m_pWindow;
	SDL_Renderer*	m_pRenderer;

	SDL_Color		m_colBkgnd;
	SDL_Color		m_colLimits;

	SDL_TimerID		m_iTimerIDtimer;

	SDL_Point		m_ptSnakeFoodLoc;
	SDL_Point		m_ptSnakePoisonLoc;

	TTF_Font*		m_pFont;

	CSnake*			m_pSnake;

	CContainer*		m_pScoreTexts;
	CContainer*		m_pTimerTexts;

	int			m_iSnakeFoodTimeout, m_iSnakePoisonTimeout;

	SDL_Texture*	m_pFoodTexture;
	SDL_Texture*	m_pPoisonTexture;
	SDL_Texture*	m_pBGTexture;
	SDL_Texture*	m_pTextGameover;
	SDL_Texture*	m_pTextPaused;
	SDL_Texture* 	m_pTitleScreen;

	SDL_Rect		m_rTextGameover;
	SDL_Rect		m_rTextPaused;
	
	CSound*			m_pSound;

	unsigned int	m_uStatus;
	int				m_iTime, m_iScore;
public:
	CApp();
	~CApp();
	int Run();
private:
	void DrawAndAnimate();
	void ResetGameState();
	static Uint32 _AppTimerTimeCallback(Uint32 interval, CApp* pApp);
};

