/*
 * SPChessUserColor.h
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#ifndef GRAPHICS_SPCHESSUSERCOLORWIN_H_
#define GRAPHICS_SPCHESSUSERCOLORWIN_H_


#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//indicator for current active user color
typedef enum{
	SP_BLACK_ACTIVE,
	SP_WHITE_ACTIVE,
}ACTIVE_COLOR;

//user color window events
typedef enum {
	SP_USER_COLOR_WHITE,
	SP_USER_COLOR_BLACK,
	SP_USER_COLOR_BACK,
	SP_USER_COLOR_START,
	SP_USER_COLOR_INVALID_ARGUMENT,
	SP_USER_COLOR_NONE,
	SP_USER_COLOR_QUIT,
} SP_USER_COLOR_EVENT;

typedef struct {
	//SDL_Window* window;
	SDL_Window* userColorWindow;
	SDL_Renderer* userColorRenderer;
	SDL_Texture* activeWhiteTexture;
	SDL_Texture* inactiveWhiteTexture;
	SDL_Texture* activeBlackTexture;
	SDL_Texture* inactiveBlackTexture;
	SDL_Texture* backTexture;
	SDL_Texture* startTexture;
	SDL_Texture* bgTexture;
	ACTIVE_COLOR activeColor; //holds the active user color button
} SPUserColorWin;

/*
 * Creates a user color window and all of its fields (textures ,Renderer..) . The window isn't shown at
 * the end of the creation.
 * The function initializes activeColor to default- SP_EASY_LEVEL_ACTIVE.
 *
 * @return
 * NULL, if an allocation error occurred,
 * An instant of a spUserColorWindowCreate otherwise.
 */
SPUserColorWin* spUserColorWindowCreate();

/*
 * Frees all memory resources associated with the source user color window.
 * If src is NULL the function does nothing.
 *
 * @param src - a pointer to the user color window to destroy
 *
 */
void spUserColorWindowDestroy(SPUserColorWin* src);

/*
 * Draws the user color window. Draws the user color's textures as active/inactive according to activeColor,
 * If src is NULL the function does nothing.
 *
 * @param src - a pointer to the user color window to draw
 */
void spUserColorWindowDraw(SPUserColorWin* src);

/*
 * Handle a given event in the user color window.
 *
 * @param src - a pointer to the user color window
 * @param event - a pointer to the event to handle
 * @return
 * SP_USER_COLOR_INVALID_ARGUMENT - if src is NULL or event is NULL
 * SP_USER_COLOR_WHITE - if the user clicked on white color button when it was inactive
 * SP_USER_COLOR_BLACK - if the user clicked on black color button when it was inactive
 * SP_USER_COLOR_BACK - if the user clicked on back button
 * SP_USER_COLOR_START - if the user clicked on start button
 * SP_USER_COLOR_QUIT - if the user clicked on quit in the window
 * SP_USER_COLOR_NONE - otherwise
 */
SP_USER_COLOR_EVENT spUserColorWindowHandleEvent(SPUserColorWin* src, SDL_Event* event);

/*
 * Hides the src user color window.
 *
 * @param src - a pointer to the user color window to hide
 */
void spUserColorWindowHide(SPUserColorWin* src);

/*
 * Shows the src user color window.
 *
 * @param src - a pointer to the user color window to show
 */
void spUserColorWindowShow(SPUserColorWin* src);

#endif /* GRAPHICS_SPCHESSUSERCOLORWIN_H_ */
