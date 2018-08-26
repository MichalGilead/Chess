/*
 * SPChessMainWin.c
 *
 *  Created on: 25 בספט׳ 2017
 *      Author: michal
 */

#include "SPChessMainWin.h"

//Inner functions
int isClickOnNewGame(int x, int y) {
	if ((x >= 325 && x <= 475) && (y >= 200 && y <= 250)) {
		return 1;
	}
	return 0;
}

int isMainClickOnLoad(int x, int y) {
	if ((x >= 325 && x <= 475) && (y >= 275 && y <= 325)) {
		return 1;
	}
	return 0;
}

int isMainClickOnQuit(int x, int y) {
	if ((x >= 325 && x <= 475) && (y >= 350 && y <= 400)) {
		return 1;
	}
	return 0;
}

SDL_Texture* initiaizeMainTexture(SPMainWin* res, char* imageAddress){
	SDL_Texture* currentTexture;
	SDL_Surface* loadingSurface = NULL; //Used as temp surface
	loadingSurface = SDL_LoadBMP(imageAddress);
	if (loadingSurface == NULL ) {
		printf("ERROR: Could not create %s surface: %s\n", imageAddress, SDL_GetError());
		spMainWindowDestroy(res);
		return NULL ;
	}
	currentTexture = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (currentTexture == NULL ) {
		printf("ERROR: Could not create %s texture: %s\n", imageAddress, SDL_GetError());
		spMainWindowDestroy(res);
		SDL_FreeSurface(loadingSurface);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return currentTexture;
}

int initializeAllMainTextures(SPMainWin* res){
	//create a background texture:
	if ((res->bgTexture = initiaizeMainTexture(res, "./graphics/images/mainBackground.bmp")) == NULL)
		return 0;
	//create a new game button texture:
	if ((res->newGameTexture = initiaizeMainTexture(res, "./graphics/images/newGameButton.bmp")) == NULL)
		return 0;
	//create a load button texture:
	if ((res->loadTexture = initiaizeMainTexture(res, "./graphics/images/loadButton.bmp")) == NULL)
		return 0;
	//create a quit button texture:
	if ((res->quitTexture = initiaizeMainTexture(res, "./graphics/images/quitButton.bmp")) == NULL)
		return 0;
	return 1;
}

SPMainWin* spMainWindowCreate() {
	SPMainWin* res = NULL;
	res = (SPMainWin*) calloc(sizeof(SPMainWin), 1);
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->mainWindow = SDL_CreateWindow("Main Menu", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->mainWindow == NULL ) {
		spMainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL ) {
		// In the case that the window could not be made...
		spMainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}

	if (!initializeAllMainTextures(res))
		return NULL;

	return res;
}

void spMainWindowDestroy(SPMainWin* src) {
	if (src==NULL) {
		return;
	}
	if (src->bgTexture != NULL ) {
		SDL_DestroyTexture(src->bgTexture);
	}
	if (src->newGameTexture != NULL ) {
		SDL_DestroyTexture(src->newGameTexture);
	}
	if (src->loadTexture != NULL ) {
		SDL_DestroyTexture(src->loadTexture);
	}
	if (src->quitTexture != NULL ) {
		SDL_DestroyTexture(src->quitTexture);
	}
	if (src->mainRenderer != NULL ) {
		SDL_DestroyRenderer(src->mainRenderer);
	}
	if (src->mainWindow != NULL ) {
		SDL_DestroyWindow(src->mainWindow);
	}

	free(src);
}

void spMainWindowDraw(SPMainWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect newGameR = { .x = 325, .y = 200, .h = 50, .w = 150 };
	SDL_Rect loadR = { .x = 325, .y = 275, .h = 50, .w = 150 };
	SDL_Rect quitR = { .x = 325, .y = 350, .h = 50, .w = 150 };
	SDL_Rect backgroundR = { .x = 0, .y = 0, .h = 600, .w = 800 };

	SDL_SetRenderDrawColor(src->mainRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->mainRenderer);
	SDL_RenderCopy(src->mainRenderer, src->bgTexture, NULL, &backgroundR);
	SDL_RenderCopy(src->mainRenderer, src->newGameTexture, NULL, &newGameR);
	SDL_RenderCopy(src->mainRenderer, src->loadTexture, NULL, &loadR);
	SDL_RenderCopy(src->mainRenderer, src->quitTexture, NULL, &quitR);
	SDL_RenderPresent(src->mainRenderer);
}

SP_MAIN_EVENT spMainWindowHandleEvent(SPMainWin* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_MAIN_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnNewGame(event->button.x, event->button.y)) {
			return SP_MAIN_NEW_GAME;
		} else if (isMainClickOnLoad(event->button.x, event->button.y)) {
			return SP_MAIN_LOAD;
		} else if (isMainClickOnQuit(event->button.x, event->button.y)) {
			return SP_MAIN_QUIT;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_MAIN_QUIT;
		}
		break;
	default:
		return SP_MAIN_NONE;
	}
	return SP_MAIN_NONE;
}

void spMainWindowHide(SPMainWin* src) {
	SDL_HideWindow(src->mainWindow);
}

void spMainWindowShow(SPMainWin* src) {
	SDL_ShowWindow(src->mainWindow);
}
