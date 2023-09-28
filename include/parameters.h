/*********************************************************************
 * parameters.h
 *
 *  Working on: 10/09/2021
 *
 *      Author: EL BAKRAOUI Salim
 *
 ********************************************************************/

#define APP_WINDOW_WIDTH			(1280)	//In pixels
#define APP_WINDOW_HEIGHT			(720)	//In pixels
#define APP_WINDOW_PADDING			(48)	//In pixels
#define APP_WINDOW_COLOR_BKGND			(SDL_Color){190,190,190,255}
#define APP_WINDOW_COLOR_LIMITS			(SDL_Color){80,80,80,255}
#define APP_TIMER_TICK_MS			(65)
#define APP_FONT_SIZE				(36)

#define APP_MAIN_FONT_FILE_NAME			"Resources/Fonts/Gameplay.ttf"
#define APP_SNAKE_HEAD_PATH			"Resources/Snake/texHead.png"
#define APP_SNAKE_BODY_PATH			"Resources/Snake/texBody.png"
#define APP_SNAKE_TAIL_PATH 			"Resources/Snake/texTail.png"

#define APP_FOOD_APPLE_PATH			"Resources/Food/apple.png"
#define APP_FOOD_POISON_PATH 			"Resources/Food/poison.png"

#define APP_DEFAULT_BG_PATH			"Resources/App/bg.jpg"
#define APP_DEFAULT_TITLE_PATH			"Resources/App/title.png"

#define SOUND_DEFAULT_COLLECT_PATH		"Resources/Sound/collect.wav"
#define SOUND_DEFAULT_POISON_PATH		"Resources/Sound/poison.wav"
#define SOUND_DEFAULT_PAUSE_PATH		"Resources/Sound/pause.wav"
#define SOUND_DEFAULT_MUSIC_PATH		"Resources/Sound/music.mp3"

#define	SNAKE_INITIAL_LENGTH			(5)	//Initial number of snake sections
#define SNAKE_SECTION_SIZE			(40)	//In pixels
#define SNAKE_SECTION_SPACING			(4)	//In pixels
#define SNAKE_POS_MIN_X				(0)	//In tabular coordinates
#define SNAKE_POS_MAX_X				((int)((APP_WINDOW_WIDTH-2*APP_WINDOW_PADDING)/(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING))) 	//In tabular coordinates
#define SNAKE_POS_MIN_Y				(0)		//In tabular coordinates
#define SNAKE_POS_MAX_Y				((int)((APP_WINDOW_HEIGHT-2*APP_WINDOW_PADDING)/(SNAKE_SECTION_SIZE+SNAKE_SECTION_SPACING)))	//In tabular coordinates

#define SNAKE_POS_INITIAL_X			(SNAKE_POS_MAX_X/2)	//In tabular coordinates
#define SNAKE_POS_INITIAL_Y			(SNAKE_POS_MAX_Y/2)	//In tabular coordinates

#define SNAKE_BODY_COLOR			(SDL_Color){150,50,50,255}
#define SNAKE_HEAD_COLOR			(SDL_Color){200,50,50,255}
#define SNAKE_TAIL_COLOR			(SDL_Color){100,50,50,255}

#define SNAKE_FOOD_SIZE				((int)(SNAKE_SECTION_SIZE*.9))			//In pixels
#define SNAKE_FOOD_PADDING			((SNAKE_SECTION_SIZE-SNAKE_FOOD_SIZE)/2)	//In pixels



