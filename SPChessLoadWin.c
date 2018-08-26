/*
 * SPChessLoadWin.c
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#include "SPChessLoadWin.h"
#include <SDL_video.h>
#include <stdio.h>



//Inner functions
int isLoadClickOnLoad(int x, int y) {
	if ((x >= 575 && x <= 725) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}

int isLoadClickOnBack(int x, int y) {
	if ((x >= 75 && x <= 225) && (y >= 400 && y <= 450)) {
		return 1;
	}
	return 0;
}

int isClickOnSlot(int x, int y, int i) {
	if ((x >= 325 && x <= 475) && (y >= 100+(i*65) && y <= 100+(i*65)+50)) {
		return 1;
	}
	return 0;
}

//Creates a texture with imageAddress.
//returns NULL if an allocation error occurred, and the new texture o.w.
SDL_Texture* initiaizeLoadTexture(SPLoadWin* res, char* imageAddress){
	SDL_Texture* currentTexture;
	SDL_Surface* loadingSurface = NULL; //Used as temp surface
	loadingSurface = SDL_LoadBMP(imageAddress); //loading the image to surface
	if (loadingSurface == NULL ) {
		printf("ERROR: Could not create %s surface: %s\n", imageAddress, SDL_GetError());
		spLoadWindowDestroy(res);
		return NULL ;
	}
	currentTexture = SDL_CreateTextureFromSurface(res->loadRenderer, loadingSurface);  //creates texture
	if (currentTexture == NULL ) {
		printf("ERROR: Could not create %s texture: %s\n", imageAddress, SDL_GetError());
		spLoadWindowDestroy(res);
		SDL_FreeSurface(loadingSurface);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return currentTexture;
}

int initializeAllLoadTextures(SPLoadWin* res){
	//create a background texture:
	if ((res->bgTexture = initiaizeLoadTexture(res,	"./graphics/images/loadBackground.bmp")) == NULL)
		return 0;
	//create a back button texture:
	if ((res->backTexture = initiaizeLoadTexture(res,	"./graphics/images/backButton.bmp")) == NULL)
		return 0;
	//create an inactive load button texture:
	if ((res->inactiveLoadTexture = initiaizeLoadTexture(res,	"./graphics/images/inactiveLoadButton.bmp")) == NULL)
		return 0;
	//create an active load button texture:
	if ((res->activeLoadTexture = initiaizeLoadTexture(res,	"./graphics/images/activeLoadButton.bmp")) == NULL)
		return 0;
	//create inactive slot 1 texture
	if ((res->slotsArr[0]->inactiveSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/slot_1Button.bmp")) == NULL)
		return 0;
	//create inactive slot 2 texture
	if ((res->slotsArr[1]->inactiveSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/slot_2Button.bmp")) == NULL)
		return 0;
	//create inactive slot 3 texture
	if ((res->slotsArr[2]->inactiveSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/slot_3Button.bmp")) == NULL)
		return 0;
	//create inactive slot 4 texture
	if ((res->slotsArr[3]->inactiveSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/slot_4Button.bmp")) == NULL)
		return 0;
	//create inactive slot 5 texture
	if ((res->slotsArr[4]->inactiveSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/slot_5Button.bmp")) == NULL)
		return 0;
	//create active slot 1 texture
	if ((res->slotsArr[0]->activeSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/markedSlot_1Button.bmp")) == NULL)
		return 0;
	//create active slot 2 texture
	if ((res->slotsArr[1]->activeSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/markedSlot_2Button.bmp")) == NULL)
		return 0;
	//create active slot 3 texture
	if ((res->slotsArr[2]->activeSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/markedSlot_3Button.bmp")) == NULL)
		return 0;
	//create active slot 4 texture
	if ((res->slotsArr[3]->activeSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/markedSlot_4Button.bmp")) == NULL)
		return 0;
	//create active slot 5 texture
	if ((res->slotsArr[4]->activeSlotTexture = initiaizeLoadTexture(res,	"./graphics/images/markedSlot_5Button.bmp")) == NULL)
		return 0;
	return 1;
}

SPLoadWin* spLoadWindowCreate() {
	FILE* ifp;
	SPLoadWin* res = NULL;
	res = (SPLoadWin*)malloc(sizeof(SPLoadWin));
	if (res == NULL ) {
		return NULL ;
	}

	res->currSlotsNum = 0; //sets current number of slots to zero

	res->activeSlotIndex = -1; //sets active slot button index to 1 (no slot has been chosen yet)
	//creates the slots array
	for (int i=0; i< SLOTS_NUMBER; i++){
		res->slotsArr[i] = (loadSlotButton*)calloc(sizeof(loadSlotButton), 1);
		if (res->slotsArr[i] == NULL ) {
			spLoadWindowDestroy(res);
			return NULL ;
		}
	}

	//sets the name for each slot
	strcpy(res->slotsArr[0]->slotName, SLOT1);
	strcpy(res->slotsArr[1]->slotName, SLOT2);
	strcpy(res->slotsArr[2]->slotName, SLOT3);
	strcpy(res->slotsArr[3]->slotName, SLOT4);
	strcpy(res->slotsArr[4]->slotName, SLOT5);

	// Create an application window with the following settings:
	res->loadWindow = SDL_CreateWindow("Load", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_HIDDEN                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->loadWindow == NULL ) {
		spLoadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->loadRenderer = SDL_CreateRenderer(res->loadWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->loadRenderer == NULL ) {
		// In the case that the window could not be made...
		spLoadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->loadActive = false; //sets load button to be inactive

	//initialize all game mode textures
	if(!initializeAllLoadTextures(res))
		return NULL;

	//opens all available slots files, while increasing the currSlotsNum accordingly
	for (int i=0; i<SLOTS_NUMBER; i++){
		ifp = fopen(res->slotsArr[i]->slotName, "r"); //opens the file to check if exists
		if (ifp!=NULL){
			res->currSlotsNum++;
			fclose(ifp);
		}
	}
	return res;
}

void spLoadWindowDestroy(SPLoadWin* src) {
	if (src==NULL) {
		return;
	}
	for (int i=0; i< SLOTS_NUMBER; i++){
		if (src->slotsArr[i] !=NULL) {
			SDL_DestroyTexture(src->slotsArr[i]->activeSlotTexture);
			SDL_DestroyTexture(src->slotsArr[i]->inactiveSlotTexture);
			free(src->slotsArr[i]);
		}
	}
	if (src->bgTexture != NULL ) {
		SDL_DestroyTexture(src->bgTexture);
	}
	if (src->backTexture!= NULL ) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->activeLoadTexture != NULL ) {
		SDL_DestroyTexture(src->activeLoadTexture);
	}
	if (src->inactiveLoadTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveLoadTexture);
	}
	if (src->loadRenderer != NULL ) {
		SDL_DestroyRenderer(src->loadRenderer);
	}
	if (src->loadWindow != NULL ) {
		SDL_DestroyWindow(src->loadWindow);
	}

	free(src);
}

void spLoadWindowDraw(SPLoadWin* src) {
	if(src==NULL){
		return;
	}
	SDL_Rect loadR = { .x = 575, .y = 400, .h = 50, .w = 150 };
	SDL_Rect backR = { .x = 75, .y = 400, .h = 50, .w = 150 };
	SDL_Rect backgroundR = { .x = 0, .y = 0, .h = 600, .w = 800 };

	SDL_SetRenderDrawColor(src->loadRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->loadRenderer);
	SDL_RenderCopy(src->loadRenderer, src->bgTexture, NULL, &backgroundR);
	SDL_RenderCopy(src->loadRenderer, src->backTexture, NULL, &backR);

	if (src->loadActive) //checks if load button is active (a slot has been chosen)
		SDL_RenderCopy(src->loadRenderer, src->activeLoadTexture, NULL, &loadR);
	else
		SDL_RenderCopy(src->loadRenderer, src->inactiveLoadTexture, NULL, &loadR);

	//for each slot that exist, checks if it is active
	for (int i=0; i<src->currSlotsNum; i++){
		SDL_Rect slotR = { .x = 325, .y = 100+(i*65), .h = 50, .w = 150 };
		if (i==src->activeSlotIndex)
			SDL_RenderCopy(src->loadRenderer, src->slotsArr[i]->activeSlotTexture, NULL, &slotR);
		else
			SDL_RenderCopy(src->loadRenderer, src->slotsArr[i]->inactiveSlotTexture, NULL, &slotR);
	}
	SDL_RenderPresent(src->loadRenderer);
}

SP_LOAD_EVENT spLoadWindowHandleEvent(SPLoadWin* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_LOAD_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		//checks if the user clicked on back button
		if (isLoadClickOnBack(event->button.x, event->button.y)) {
			return SP_LOAD_BACK;
		}
		//checks if the user clicked on load button when it was active
		else if (src->loadActive && isLoadClickOnLoad(event->button.x, event->button.y)) {
			return SP_LOAD_LOAD;
		}
		//checks for each slot that exist if the user clicked on it
		else{
			for (int i=0; i<src->currSlotsNum; i++){
				if (isClickOnSlot(event->button.x, event->button.y, i)) {
					if (i==src->activeSlotIndex)
						return SP_LOAD_SLOT;
					src->activeSlotIndex = i; //changes activeSlotIndex accordingly
					src->loadActive = true; //changes loadActive to be active
					return SP_LOAD_SLOT;
				}
			}
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) { //user clicked on quit in the window
			return SP_LOAD_QUIT;
		}
		break;
	default:
		return SP_LOAD_NONE;
	}
	return SP_LOAD_NONE;
}

void spLoadWindowHide(SPLoadWin* src) {
	SDL_HideWindow(src->loadWindow);
}

void spLoadWindowShow(SPLoadWin* src) {
	SDL_ShowWindow(src->loadWindow);
}

