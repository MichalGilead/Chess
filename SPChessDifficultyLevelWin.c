/*
 * SPChessDifficultyLevelWin.c
 *
 *  Created on: 25 � 2017
 *      Author: michal
 */

#include "SPChessDifficultyLevelWin.h"
#include <SDL_video.h>
#include <stdio.h>


//Inner functions
int isClickNoob(int x, int y) {
	if ((x >= 150 && x <= 475) && (y >= 100 && y <= 150)) {
		return 1;
	}
	return 0;
}

int isClickOnEasy(int x, int y) {
	if ((x >= 150 && x <= 475) && (y >= 165 && y <= 215)) {
		return 1;
	}
	return 0;
}
int isClickOnModerate(int x, int y) {
	if ((x >= 150 && x <= 475) && (y >= 230 && y <= 280)) {
		return 1;
	}
	return 0;
}
int isClickOnHard(int x, int y) {
	if ((x >= 150 && x <= 475) && (y >= 295 && y <= 345)) {
		return 1;
	}
	return 0;
}

int isDifficultyLevelClickOnBack(int x, int y) {
	if ((x >= 75 && x <= 225) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}

int isClickOnNext(int x, int y) {
	if ((x >= 575 && x <= 725) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}

//Creates a texture with imageAddress.
//returns NULL if an allocation error occurred, and the new texture o.w.
SDL_Texture* initiaizeDifficultyLevelTexture(SPDifficultyLevelWin* res, char* imageAddress){
	SDL_Texture* currentTexture;
	SDL_Surface* loadingSurface = NULL; //Used as temp surface
	loadingSurface = SDL_LoadBMP(imageAddress); //loading the image to surface
	if (loadingSurface == NULL ) {
		printf("ERROR: Could not create %s surface: %s\n", imageAddress, SDL_GetError());
		spDifficultyLevelWindowDestroy(res);
		return NULL ;
	}
	currentTexture = SDL_CreateTextureFromSurface(res->difficultyLevelRenderer, loadingSurface); //creates texture
	if (currentTexture == NULL ) {
		printf("ERROR: Could not create %s texture: %s\n", imageAddress, SDL_GetError());
		spDifficultyLevelWindowDestroy(res);
		SDL_FreeSurface(loadingSurface);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return currentTexture;
}

bool initializeAllDifficultyLevelTextures(SPDifficultyLevelWin* res){
	//create a background texture:
	if ((res->bgTexture = initiaizeDifficultyLevelTexture(res,	"./graphics/images/difficultyLevelBackground.bmp")) == NULL)
		return false;
	//create an active noob texture:
	if ((res->activeNoobTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/activeNoobButton.bmp")) == NULL)
		return false;
	//create an inactive noob texture:
	if (( res->inactiveNoobTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/inactiveNoobButton.bmp")) == NULL)
		return false;
	//create an active easy texture:
	if ((res->activeEasyTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/activeEasyButton.bmp")) == NULL)
		return false;
	//create an inactive easy texture:
	if ((res->inactiveEasyTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/inactiveEasyButton.bmp")) == NULL)
		return false;
	//create an active moderate texture:
	if ((res->activeModerateTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/activeModerateButton.bmp")) == NULL)
		return false;
	//create an inactive moderate texture:
	if ((res->inactiveModerateTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/inactiveModerateButton.bmp")) == NULL)
		return false;
	//create an active hard texture:
	if ((res->activeHardTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/activeHardButton.bmp")) == NULL)
		return false;
	//create an inactive hard texture:
	if ((res->inactiveHardTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/inactiveHardButton.bmp")) == NULL)
		return false;
	//create a back texture:
	if ((res->backTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/backButton.bmp")) == NULL)
		return false;
	//create an active next texture:
	if ((res->nextTexture = initiaizeDifficultyLevelTexture(res, "./graphics/images/activeNextButton.bmp")) == NULL)
		return false;

	return true;
}


SPDifficultyLevelWin* spDifficultyLevelWindowCreate() {
	SPDifficultyLevelWin* res = NULL;
	res = (SPDifficultyLevelWin*) calloc(sizeof(SPDifficultyLevelWin), 1);
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->difficultyLevelWindow = SDL_CreateWindow("Difficulty Level", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_HIDDEN                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->difficultyLevelWindow == NULL ) {
		spDifficultyLevelWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}

	res->difficultyLevelRenderer = SDL_CreateRenderer(res->difficultyLevelWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->difficultyLevelRenderer == NULL ) {
		// In the case that the window could not be made...
		spDifficultyLevelWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}

	res->activeLevel = SP_EASY_LEVEL_ACTIVE; //sets activeLevel to default

	//initialize all game mode textures
	if (!initializeAllDifficultyLevelTextures(res))
		return NULL;

	return res;
}

void spDifficultyLevelWindowDestroy(SPDifficultyLevelWin* src) {
	if (src==NULL) {
		return;
	}
	if (src->bgTexture != NULL ) {
		SDL_DestroyTexture(src->bgTexture);
	}
	if (src->activeNoobTexture != NULL ) {
		SDL_DestroyTexture(src->activeNoobTexture);
	}
	if (src->inactiveNoobTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveNoobTexture);
	}
	if (src->activeEasyTexture != NULL ) {
		SDL_DestroyTexture(src->activeEasyTexture);
	}
	if (src->inactiveEasyTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveEasyTexture);
	}
	if (src->activeModerateTexture != NULL ) {
		SDL_DestroyTexture(src->activeModerateTexture);
	}
	if (src->inactiveModerateTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveModerateTexture);
	}
	if (src->activeHardTexture != NULL ) {
		SDL_DestroyTexture(src->activeHardTexture);
	}
	if (src->inactiveHardTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveHardTexture);
	}
	if (src->backTexture != NULL ) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->nextTexture != NULL ) {
		SDL_DestroyTexture(src->nextTexture);
	}
	if (src->difficultyLevelRenderer != NULL ) {
		SDL_DestroyRenderer(src->difficultyLevelRenderer);
	}
	if (src->difficultyLevelWindow != NULL ) {
		SDL_DestroyWindow(src->difficultyLevelWindow);
	}
	free(src);
}

void spDifficultyLevelWindowDraw(SPDifficultyLevelWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect noobR = { .x = 325, .y = 100, .h = 50, .w = 150 };
	SDL_Rect easyR = { .x = 325, .y = 165, .h = 50, .w = 150 };
	SDL_Rect moderateR = { .x = 325, .y = 230, .h = 50, .w = 150 };
	SDL_Rect hardR = { .x = 325, .y = 295, .h = 50, .w = 150 };
	SDL_Rect backR = { .x = 75, .y = 400, .h = 50, .w = 150 };
	SDL_Rect nextR = { .x = 575, .y = 400, .h = 50, .w = 150 };
	SDL_Rect backgroundR = { .x = 0, .y = 0, .h = 600, .w = 800 };

	SDL_SetRenderDrawColor(src->difficultyLevelRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->difficultyLevelRenderer);
	SDL_RenderCopy(src->difficultyLevelRenderer, src->bgTexture, NULL, &backgroundR);
	SDL_RenderCopy(src->difficultyLevelRenderer, src->backTexture, NULL, &backR);
	SDL_RenderCopy(src->difficultyLevelRenderer, src->nextTexture, NULL, &nextR);


	if (src->activeLevel == SP_NOOB_LEVEL_ACTIVE) //checks if noob level is active (has been chosen)
		SDL_RenderCopy(src->difficultyLevelRenderer, src->activeNoobTexture, NULL, &noobR);
	else
		SDL_RenderCopy(src->difficultyLevelRenderer, src->inactiveNoobTexture, NULL, &noobR);

	if (src->activeLevel == SP_EASY_LEVEL_ACTIVE) //checks if easy level is active (has been chosen)
		SDL_RenderCopy(src->difficultyLevelRenderer, src->activeEasyTexture, NULL, &easyR);
	else
		SDL_RenderCopy(src->difficultyLevelRenderer, src->inactiveEasyTexture, NULL, &easyR);

	if (src->activeLevel == SP_MODERATE_LEVEL_ACTIVE) //checks if moderate level is active (has been chosen)
		SDL_RenderCopy(src->difficultyLevelRenderer, src->activeModerateTexture, NULL, &moderateR);
	else
		SDL_RenderCopy(src->difficultyLevelRenderer, src->inactiveModerateTexture, NULL, &moderateR);

	if (src->activeLevel == SP_HARD_LEVEL_ACTIVE) //checks if hard level is active (has been chosen)
			SDL_RenderCopy(src->difficultyLevelRenderer, src->activeHardTexture, NULL, &hardR);
		else
			SDL_RenderCopy(src->difficultyLevelRenderer, src->inactiveHardTexture, NULL, &hardR);

	SDL_RenderPresent(src->difficultyLevelRenderer);
}


SP_DIFFICULTY_LEVEL_EVENT spDifficultyLevelWindowHandleEvent(SPDifficultyLevelWin* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_DIFFICULTY_LEVEL_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		//checks if the user clicked on noob level button when it was inactive
		if (src->activeLevel != SP_NOOB_LEVEL_ACTIVE && isClickNoob(event->button.x, event->button.y)) {
			src->activeLevel = SP_NOOB_LEVEL_ACTIVE; //changes activeLevel to noob
			return SP_DIFFICULTY_LEVEL_NOOB;
		}
		//checks if the user clicked on easy level button when it was inactive
		else if (src->activeLevel != SP_EASY_LEVEL_ACTIVE && isClickOnEasy(event->button.x, event->button.y)) {
			src->activeLevel = SP_EASY_LEVEL_ACTIVE; //changes activeLevel to easy
			return SP_DIFFICULTY_LEVEL_EASY;
		}
		//checks if the user clicked on moderate level button when it was inactive
		else if (src->activeLevel != SP_MODERATE_LEVEL_ACTIVE && isClickOnModerate(event->button.x, event->button.y)) {
			src->activeLevel = SP_MODERATE_LEVEL_ACTIVE; //changes activeLevel to moderate
			return SP_DIFFICULTY_LEVEL_MODERATE;
		}
		//checks if the user clicked on hard level button when it was inactive
		else if (src->activeLevel != SP_HARD_LEVEL_ACTIVE && isClickOnHard(event->button.x, event->button.y)) {
			src->activeLevel = SP_HARD_LEVEL_ACTIVE; //changes activeLevel to hard
			return SP_DIFFICULTY_LEVEL_HARD;
		}
		//checks if the user clicked on back button
		else if (isDifficultyLevelClickOnBack(event->button.x, event->button.y)) {
			return SP_DIFFICULTY_LEVEL_BACK;
		}
		//checks if the user clicked on next button
		else if (isClickOnNext(event->button.x, event->button.y)) {
			return SP_DIFFICULTY_LEVEL_NEXT;
		}

		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) { //user clicked on quit in the window
			return SP_DIFFICULTY_LEVEL_QUIT;
		}
		break;
	default:
		return SP_DIFFICULTY_LEVEL_NONE;
	}
	return SP_DIFFICULTY_LEVEL_NONE;
}

void spDifficultyLevelWindowHide(SPDifficultyLevelWin* src) {
	SDL_HideWindow(src->difficultyLevelWindow);
}

void spDifficultyLevelWindowShow(SPDifficultyLevelWin* src) {
	SDL_ShowWindow(src->difficultyLevelWindow);
}


