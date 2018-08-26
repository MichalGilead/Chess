/*
 * SPChessGameModeWin.c
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#include "SPChessGameModeWin.h"
#include <SDL_video.h>
#include <stdio.h>


//Inner functions
int isClickOnOnePlayer(int x, int y) {
	if ((x >= 150 && x <= 300) && (y >= 200 && y <= 250)) {
		return 1;
	}
	return 0;
}

int isClickOnTwoPlayer(int x, int y) {
	if ((x >= 500 && x <= 650) && (y >= 200 && y <= 250)) {
		return 1;
	}
	return 0;
}

int isGameModeClickOnBack(int x, int y) {
	if ((x >= 75 && x <= 225) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}

int isClickOnNextOrStart(int x, int y) {
	if ((x >= 575 && x <= 725) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}

//Creates a texture with imageAddress.
//returns NULL if an allocation error occurred, and the new texture o.w.
SDL_Texture* initiaizeGameModeTexture(SPGameModeWin* res, char* imageAddress){
	SDL_Texture* currentTexture;
	SDL_Surface* loadingSurface = NULL; //used as temp surface
	loadingSurface = SDL_LoadBMP(imageAddress); //loading the image to surface
	if (loadingSurface == NULL ) {
		printf("ERROR: Could not create %s surface: %s\n", imageAddress, SDL_GetError());
		spGameModeWindowDestroy(res);
		return NULL ;
	}
	currentTexture = SDL_CreateTextureFromSurface(res->gameModeRenderer, loadingSurface); //creates texture
	if (currentTexture == NULL ) {
		printf("ERROR: Could not create %s texture: %s\n", imageAddress, SDL_GetError());
		spGameModeWindowDestroy(res);
		SDL_FreeSurface(loadingSurface);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return currentTexture;
}

int initializeAllGameModeTextures(SPGameModeWin* res){
	//create a background texture:
	if ((res->bgTexture = initiaizeGameModeTexture(res,	"./graphics/images/gameModeBackground.bmp")) == NULL)
		return 0;
	//create an inactive onePlayer texture:
	if ((res->inactiveOnePlayerTexture = initiaizeGameModeTexture(res,	"./graphics/images/inactiveOnePlayerButton.bmp")) == NULL)
		return 0;
	//create an active onePlayer texture:
	if ((res->activeOnePlayerTexture = initiaizeGameModeTexture(res,	"./graphics/images/activeOnePlayerButton.bmp")) == NULL)
		return 0;
	//create an inactive twoPlayer texture:
	if ((res->inactiveTwoPlayerTexture = initiaizeGameModeTexture(res,	"./graphics/images/inactiveTwoPlayerButton.bmp")) == NULL)
		return 0;
	//create an active twoPlayer texture:
	if ((res->activeTwoPlayerTexture = initiaizeGameModeTexture(res,	"./graphics/images/activeTwoPlayerButton.bmp")) == NULL)
		return 0;
	//create a back texture:
	if ((res->backTexture = initiaizeGameModeTexture(res,	"./graphics/images/backButton.bmp")) == NULL)
		return 0;
	//create a next texture:
	if ((res->nextTexture = initiaizeGameModeTexture(res,	"./graphics/images/nextButton.bmp")) == NULL)
		return 0;
	//create a start texture:
	if ((res->startTexture = initiaizeGameModeTexture(res,	"./graphics/images/startButton.bmp")) == NULL)
		return 0;
	return 1;
}

SPGameModeWin* spGameModeWindowCreate() {
	SPGameModeWin* res = NULL;
	res = (SPGameModeWin*) calloc(sizeof(SPGameModeWin), 1);
	if (res == NULL ) {
		return NULL ;
	}

	//Creates an application window with the following settings:
	res->gameModeWindow = SDL_CreateWindow("Game Mode", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_HIDDEN                  // flags - see below
			);

	//Checks that the window was successfully created
	if (res->gameModeWindow == NULL ) {
		spGameModeWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->gameModeRenderer = SDL_CreateRenderer(res->gameModeWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->gameModeRenderer == NULL ) {
		//In the case that the window could not be made...
		spGameModeWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}

	res->activeGameMode = SP_1_PLAYER_GAME_MODE_ACTIVE; //sets activeGameMode to default
	res->nextIsActive = true; //indicates that the next button should be drawn

	//initialize all game mode textures
	if (!initializeAllGameModeTextures(res))
		return NULL;

	return res;
}

void spGameModeWindowDestroy(SPGameModeWin* src) {
	if (src==NULL) {
		return;
	}
	if (src->bgTexture != NULL ) {
		SDL_DestroyTexture(src->bgTexture);
	}
	if (src->activeOnePlayerTexture != NULL ) {
		SDL_DestroyTexture(src->activeOnePlayerTexture);
	}
	if (src->inactiveOnePlayerTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveOnePlayerTexture);
	}
	if (src->activeTwoPlayerTexture != NULL ) {
		SDL_DestroyTexture(src->activeTwoPlayerTexture);
	}
	if (src->inactiveTwoPlayerTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveTwoPlayerTexture);
	}
	if (src->backTexture != NULL ) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->nextTexture != NULL ) {
		SDL_DestroyTexture(src->nextTexture);
	}
	if (src->startTexture != NULL ) {
		SDL_DestroyTexture(src->startTexture);
	}
	if (src->gameModeRenderer != NULL ) {
		SDL_DestroyRenderer(src->gameModeRenderer);
	}
	if (src->gameModeWindow != NULL ) {
		SDL_DestroyWindow(src->gameModeWindow);
	}

	free(src);
}

void spGameModeWindowDraw(SPGameModeWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect onePlayerR = { .x = 150, .y = 200, .h = 50, .w = 150 };
	SDL_Rect twoPlayerR = { .x = 500, .y = 200, .h = 50, .w = 150 };
	SDL_Rect backR = { .x = 75, .y = 400, .h = 50, .w = 150 };
	SDL_Rect nextOrStartR = { .x = 575, .y = 400, .h = 50, .w = 150 };
	SDL_Rect backgroundR = { .x = 0, .y = 0, .h = 600, .w = 800 };

	SDL_SetRenderDrawColor(src->gameModeRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->gameModeRenderer);
	SDL_RenderCopy(src->gameModeRenderer, src->bgTexture, NULL, &backgroundR);
	SDL_RenderCopy(src->gameModeRenderer, src->backTexture, NULL, &backR);

	if (src->activeGameMode == SP_1_PLAYER_GAME_MODE_ACTIVE) //checks if 1 player mode is active (has been chosen)
		SDL_RenderCopy(src->gameModeRenderer, src->activeOnePlayerTexture, NULL, &onePlayerR);
	else
		SDL_RenderCopy(src->gameModeRenderer, src->inactiveOnePlayerTexture, NULL, &onePlayerR);

	if (src->activeGameMode == SP_2_PLAYER_GAME_MODE_ACTIVE) //checks if 2 players mode is active (has been chosen)
		SDL_RenderCopy(src->gameModeRenderer, src->activeTwoPlayerTexture, NULL, &twoPlayerR);
	else
		SDL_RenderCopy(src->gameModeRenderer, src->inactiveTwoPlayerTexture, NULL, &twoPlayerR);

	if (src->nextIsActive) //checks whether next or start is active
		SDL_RenderCopy(src->gameModeRenderer, src->nextTexture, NULL, &nextOrStartR);
	else
		SDL_RenderCopy(src->gameModeRenderer, src->startTexture, NULL, &nextOrStartR);

	SDL_RenderPresent(src->gameModeRenderer);
}

SP_GAME_MODE_EVENT spGameModeWindowHandleEvent(SPGameModeWin* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_GAME_MODE_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		//checks if the user clicked on one player mode button when it was inactive
		if (src->activeGameMode != SP_1_PLAYER_GAME_MODE_ACTIVE && isClickOnOnePlayer(event->button.x, event->button.y)) {
			src->nextIsActive = true; //changes next to be active
			src->activeGameMode = SP_1_PLAYER_GAME_MODE_ACTIVE; //changes activeGameMode accordingly
			return SP_GAME_MODE_ONE_PLAYER;
		}
		//checks if the user clicked on two players mode button when it was inactive
		else if (src->activeGameMode != SP_2_PLAYER_GAME_MODE_ACTIVE && isClickOnTwoPlayer(event->button.x, event->button.y)) {
			src->nextIsActive = false; //changes start to be active
			src->activeGameMode = SP_2_PLAYER_GAME_MODE_ACTIVE; //changes activeGameMode accordingly
			return SP_GAME_MODE_TWO_PLAYER;
		}
		//checks if the user clicked on back button
		else if (isGameModeClickOnBack(event->button.x, event->button.y)) {
			return SP_GAME_MODE_BACK;
		}
		//checks if the user clicked on next button when it was active
		else if (src->nextIsActive && isClickOnNextOrStart(event->button.x, event->button.y)) {
			return SP_GAME_MODE_NEXT;
		}
		//checks if the user clicked on start button when it was active
		else if (!src->nextIsActive && isClickOnNextOrStart(event->button.x, event->button.y)) {
			return SP_GAME_MODE_START;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) { //user clicked on quit in the window
			return SP_GAME_MODE_QUIT;
		}
		break;
	default:
		return SP_GAME_MODE_NONE;
	}
	return SP_GAME_MODE_NONE;
}

void spGameModeWindowHide(SPGameModeWin* src) {
	SDL_HideWindow(src->gameModeWindow);
}

void spGameModeWindowShow(SPGameModeWin* src) {
	SDL_ShowWindow(src->gameModeWindow);
}


