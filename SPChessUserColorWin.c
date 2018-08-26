/*
 * SPChessUserColor.c
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#include "SPChessUserColorWin.h"
#include <SDL_video.h>

//Inner functions
int isClickOnWhite(int x, int y) {
	if ((x >= 150 && x <= 300) && (y >= 200 && y <= 250)) {
		return 1;
	}
	return 0;
}

int isClickOnBlack(int x, int y) {
	if ((x >= 500 && x <= 650) && (y >= 200 && y <= 250)) {
		return 1;
	}
	return 0;
}

int isUserColorClickOnBack(int x, int y) {
	if ((x >= 75 && x <= 225) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}

int isClickOnStart(int x, int y) {
	if ((x >= 575 && x <= 725) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}

//Creates a texture with imageAddress.
//returns NULL if an allocation error occurred, and the new texture o.w.
SDL_Texture* initiaizeUserColorTexture(SPUserColorWin* res, char* imageAddress){
	SDL_Texture* currentTexture;
	SDL_Surface* loadingSurface = NULL; //Used as temp surface
	loadingSurface = SDL_LoadBMP(imageAddress); //loading the image to surface
	if (loadingSurface == NULL ) {
		printf("ERROR: Could not create %s surface: %s\n", imageAddress, SDL_GetError());
		spUserColorWindowDestroy(res);
		return NULL ;
	}
	currentTexture = SDL_CreateTextureFromSurface(res->userColorRenderer, loadingSurface); //creates texture
	if (currentTexture == NULL ) {
		printf("ERROR: Could not create %s texture: %s\n", imageAddress, SDL_GetError());
		spUserColorWindowDestroy(res);
		SDL_FreeSurface(loadingSurface);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return currentTexture;
}

int initializeAllUserColorTextures(SPUserColorWin* res){
	//create a background texture:
	if ((res->bgTexture = initiaizeUserColorTexture(res, "./graphics/images/userColorBackground.bmp")) == NULL)
		return 0;
	//create an active white texture:
	if ((res->activeWhiteTexture = initiaizeUserColorTexture(res, "./graphics/images/activeWhiteButton.bmp")) == NULL)
		return 0;
	//create an inactive white texture:
	if ((res->inactiveWhiteTexture = initiaizeUserColorTexture(res, "./graphics/images/inactiveWhiteButton.bmp")) == NULL)
		return 0;
	//create an active black texture:
	if ((res->activeBlackTexture = initiaizeUserColorTexture(res, "./graphics/images/activeBlackButton.bmp")) == NULL)
		return 0;
	//create an inactive black texture:
	if ((res->inactiveBlackTexture = initiaizeUserColorTexture(res, "./graphics/images/inactiveBlackButton.bmp")) == NULL)
		return 0;
	//create an back texture:
	if ((res->backTexture = initiaizeUserColorTexture(res, "./graphics/images/backButton.bmp")) == NULL)
		return 0;
	//create a start texture:
	if ((res->startTexture = initiaizeUserColorTexture(res, "./graphics/images/activeStartButton.bmp")) == NULL)
		return 0;
	return 1;
}

SPUserColorWin* spUserColorWindowCreate() {
	SPUserColorWin* res = NULL;
	res = (SPUserColorWin*) calloc(sizeof(SPUserColorWin), 1);
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->userColorWindow = SDL_CreateWindow("User Color", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_HIDDEN                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->userColorWindow == NULL ) {
		spUserColorWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->userColorRenderer = SDL_CreateRenderer(res->userColorWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->userColorRenderer == NULL ) {
		// In the case that the window could not be made...
		spUserColorWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->activeColor = SP_WHITE_ACTIVE; //sets activeColor to default

	//initialize all game mode textures
	if (!initializeAllUserColorTextures(res))
		return NULL;

	return res;
}

void spUserColorWindowDestroy(SPUserColorWin* src) {
	if (src==NULL) {
		return;
	}
	if (src->bgTexture != NULL ) {
		SDL_DestroyTexture(src->bgTexture);
	}
	if (src->activeWhiteTexture != NULL ) {
		SDL_DestroyTexture(src->activeWhiteTexture);
	}
	if (src->inactiveWhiteTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveWhiteTexture);
	}
	if (src->activeBlackTexture != NULL ) {
		SDL_DestroyTexture(src->activeBlackTexture);
	}
	if (src->inactiveBlackTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveBlackTexture);
	}
	if (src->backTexture != NULL ) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->startTexture != NULL ) {
		SDL_DestroyTexture(src->startTexture);
	}
	if (src->userColorRenderer != NULL ) {
		SDL_DestroyRenderer(src->userColorRenderer);
	}
	if (src->userColorWindow != NULL ) {
		SDL_DestroyWindow(src->userColorWindow);
	}

	free(src);
}

void spUserColorWindowDraw(SPUserColorWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect whiteR = { .x = 150, .y = 200, .h = 50, .w = 150 };
	SDL_Rect blackR = { .x = 500, .y = 200, .h = 50, .w = 150 };
	SDL_Rect backR = { .x = 75, .y = 400, .h = 50, .w = 150 };
	SDL_Rect startR = { .x = 575, .y = 400, .h = 50, .w = 150 };
	SDL_Rect backgroundR = { .x = 0, .y = 0, .h = 600, .w = 800 };

	SDL_SetRenderDrawColor(src->userColorRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->userColorRenderer);
	SDL_RenderCopy(src->userColorRenderer, src->bgTexture, NULL, &backgroundR);
	SDL_RenderCopy(src->userColorRenderer, src->backTexture, NULL, &backR);
	SDL_RenderCopy(src->userColorRenderer, src->startTexture, NULL, &startR);

	if (src->activeColor == SP_WHITE_ACTIVE) //checks if white color is active (has been chosen)
		SDL_RenderCopy(src->userColorRenderer, src->activeWhiteTexture, NULL, &whiteR);
	else
		SDL_RenderCopy(src->userColorRenderer, src->inactiveWhiteTexture, NULL, &whiteR);

	if (src->activeColor == SP_BLACK_ACTIVE) //checks if black color is active (has been chosen)
		SDL_RenderCopy(src->userColorRenderer, src->activeBlackTexture, NULL, &blackR);
	else
		SDL_RenderCopy(src->userColorRenderer, src->inactiveBlackTexture, NULL, &blackR);

	SDL_RenderPresent(src->userColorRenderer);
}

SP_USER_COLOR_EVENT spUserColorWindowHandleEvent(SPUserColorWin* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_USER_COLOR_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		//checks if the user clicked on white color button when it was inactive
		if (src->activeColor != SP_WHITE_ACTIVE && isClickOnWhite(event->button.x, event->button.y)) {
			src->activeColor = SP_WHITE_ACTIVE; //changes activeColor to white
			return SP_USER_COLOR_WHITE;
		}
		//checks if the user clicked on black color button when it was inactive
		else if (src->activeColor != SP_BLACK_ACTIVE && isClickOnBlack(event->button.x, event->button.y)) {
			src->activeColor = SP_BLACK_ACTIVE; //changes activeColor to black
			return SP_USER_COLOR_BLACK;
		}
		//checks if the user clicked on back button
		else if (isUserColorClickOnBack(event->button.x, event->button.y)) {
			return SP_USER_COLOR_BACK;
		}
		//checks if the user clicked on start button
		else if (isClickOnStart(event->button.x, event->button.y)) {
			return SP_USER_COLOR_START;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_USER_COLOR_QUIT;
		}
		break;
	default:
		return SP_USER_COLOR_NONE;
	}
	return SP_USER_COLOR_NONE;
}

void spUserColorWindowHide(SPUserColorWin* src) {
	SDL_HideWindow(src->userColorWindow);
}

void spUserColorWindowShow(SPUserColorWin* src) {
	SDL_ShowWindow(src->userColorWindow);
}


