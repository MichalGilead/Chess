/*
 * SPChessMainWin.h
 *
 *  Created on: 25 בספט׳ 2017
 *      Author: michal
 */

#ifndef SPCHESSMAINWIN_H_
#define SPCHESSMAINWIN_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
	SP_MAIN_NEW_GAME,
	SP_MAIN_LOAD,
	SP_MAIN_QUIT,
	SP_MAIN_NONE,
	SP_MAIN_INVALID_ARGUMENT
} SP_MAIN_EVENT;

typedef struct {
	//SDL_Window* window;
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	SDL_Texture* quitTexture;
	SDL_Texture* loadTexture;
	SDL_Texture* newGameTexture;
	SDL_Texture* bgTexture;
} SPMainWin;

SPMainWin* spMainWindowCreate();
void spMainWindowDraw(SPMainWin* src);
void spMainWindowDestroy(SPMainWin* src);
void spMainWindowHide(SPMainWin* src);
void spMainWindowShow(SPMainWin* src);
SP_MAIN_EVENT spMainWindowHandleEvent(SPMainWin* src, SDL_Event* event);

#endif /* SPCHESSMAINWIN_H_ */
