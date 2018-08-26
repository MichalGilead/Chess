/*
 * SPChessLoadWin.h
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#ifndef GRAPHICS_SPCHESSLOADWIN_H_
#define GRAPHICS_SPCHESSLOADWIN_H_

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SP_MAX_LINE_LENGTH 1024
#define SLOTS_NUMBER 5
//slot's name macros
#define SLOT1 "slot_1.xml"
#define SLOT2 "slot_2.xml"
#define SLOT3 "slot_3.xml"
#define SLOT4 "slot_4.xml"
#define SLOT5 "slot_5.xml"

//load window events
typedef enum {
	SP_LOAD_LOAD,
	SP_LOAD_BACK,
	SP_LOAD_SLOT,
	SP_LOAD_QUIT,
	SP_LOAD_NONE,
	SP_LOAD_INVALID_ARGUMENT
} SP_LOAD_EVENT;

//a stucture that is used to represent a slot button.
typedef struct{
	SDL_Texture* inactiveSlotTexture;
	SDL_Texture* activeSlotTexture;
	char slotName[SP_MAX_LINE_LENGTH]; //slot's name
} loadSlotButton;


typedef struct {
	SDL_Window* loadWindow;
	SDL_Renderer* loadRenderer;
	loadSlotButton* slotsArr[SLOTS_NUMBER]; //array for all slots
	int currSlotsNum; //current number of slots available
	int activeSlotIndex; //holds the active slot button index (the chosen one)
	SDL_Texture* activeLoadTexture;
	SDL_Texture* inactiveLoadTexture;
	bool loadActive; //true if load is active, false o.w.
	SDL_Texture* backTexture;
	SDL_Texture* bgTexture;
} SPLoadWin;

/*
 * Creates a load window and all of its fields (textures ,Renderer..) . The window isn't shown at
 * the end of the creation.
 * The function initializes currSlotsNum to the number of files exists, activeSlotIndex to -1 as
 * no slot has been chosen yet, and loadActive to false - load button to be inactive
 *
 * @return
 * NULL, if an allocation error occurred,
 * An instant of a spLoadWindowCreate otherwise.
 */
SPLoadWin* spLoadWindowCreate();

/*
 * Frees all memory resources associated with the source load window.
 * If src is NULL the function does nothing.
 *
 * @param src - a pointer to the load window to destroy
 *
 */
void spLoadWindowDestroy(SPLoadWin* src);

/*
 * Draws the load window. Draws the slots that their files exist as active/inactive according to activeSlotIndex,
 * and load's button as active/inactive according to loadActive.
 * If src is NULL the function does nothing.
 *
 * @param src - a pointer to the load window to draw
 */
void spLoadWindowDraw(SPLoadWin* src);

/*
 * Handle a given event in the load window.
 *
 * @param src - a pointer to the load window
 * @param event - a pointer to the event to handle
 * @return
 * SP_LOAD_INVALID_ARGUMENT - if src is NULL or event is NULL
 * SP_LOAD_BACK - if the user clicked on back button
 * SP_LOAD_LOAD - if the user clicked on load button when it was active
 * SP_LOAD_SLOT - if the user clicked an available slot
 * SP_LOAD_QUIT - if the user clicked on quit in the window
 * SP_LOAD_NONE - otherwise
 */
SP_LOAD_EVENT spLoadWindowHandleEvent(SPLoadWin* src, SDL_Event* event);

/*
 * Hides the src load window.
 *
 * @param src - a pointer to the load window to hide
 */
void spLoadWindowHide(SPLoadWin* src);

/*
 * Shows the src load window.
 *
 * @param src - a pointer to the load window to show
 */
void spLoadWindowShow(SPLoadWin* src);



#endif /* GRAPHICS_SPCHESSLOADWIN_H_ */
