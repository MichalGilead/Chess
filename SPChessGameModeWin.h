/*
 * SPChessGameModeWin.h
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#ifndef GRAPHICS_SPCHESSGAMEMODEWIN_H_
#define GRAPHICS_SPCHESSGAMEMODEWIN_H_

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//indicator for current active game mode
typedef enum{
	SP_1_PLAYER_GAME_MODE_ACTIVE,
	SP_2_PLAYER_GAME_MODE_ACTIVE,
}ACTIVE_GAME_MODE;

//game mode window events
typedef enum {
	SP_GAME_MODE_ONE_PLAYER,
	SP_GAME_MODE_TWO_PLAYER,
	SP_GAME_MODE_BACK,
	SP_GAME_MODE_NEXT,
	SP_GAME_MODE_START,
	SP_GAME_MODE_INVALID_ARGUMENT,
	SP_GAME_MODE_NONE,
	SP_GAME_MODE_QUIT,
} SP_GAME_MODE_EVENT;

//a structure that holds all fields of a game mode window
typedef struct {
	SDL_Window* gameModeWindow;
	SDL_Renderer* gameModeRenderer;
	SDL_Texture* activeOnePlayerTexture;
	SDL_Texture* inactiveOnePlayerTexture;
	SDL_Texture* activeTwoPlayerTexture;
	SDL_Texture* inactiveTwoPlayerTexture;
	SDL_Texture* backTexture;
	SDL_Texture* nextTexture;
	SDL_Texture* startTexture;
	SDL_Texture* bgTexture;
	ACTIVE_GAME_MODE activeGameMode; //holds the active game mode button
	bool nextIsActive; //true if next is active and drawn, false if start is active and drawn
} SPGameModeWin;

/*
 * Creates a game mode window and all of its fields (textures ,Renderer..) . The window isn't shown at
 * the end of the creation.
 * The function initializes activeGameMode to default- SP_1_PLAYER_GAME_MODE_ACTIVE,
 * and nextIsActive to true.
 *
 * @return
 * NULL, if an allocation error occurred,
 * An instant of a SPGameModeWin otherwise.
 */
SPGameModeWin* spGameModeWindowCreate();

/*
 * Frees all memory resources associated with the source game mode window.
 * If src is NULL the function does nothing.
 *
 * @param src - a pointer to the game mode window to destroy
 *
 */
void spGameModeWindowDestroy(SPGameModeWin* src);

/*
 * Draws the game mode window. Draws the game mode's textures as active/inactive according to activeGameMode,
 * and next or start according to nextIsActive (next if true, false o.w.)
 * If src is NULL the function does nothing.
 *
 * @param src - a pointer to the game mode window to draw
 */
void spGameModeWindowDraw(SPGameModeWin* src);

/*
 * Handle a given event in the game mode window.
 *
 * @param src - a pointer to the game mode window
 * @param event - a pointer to the event to handle
 * @return
 * SP_GAME_MODE_INVALID_ARGUMENT - if src is NULL or event is NULL
 * SP_GAME_MODE_ONE_PLAYER - if the user clicked on one player mode button when it was inactive
 * SP_GAME_MODE_TWO_PLAYER - if the user clicked on two players mode button when it was inactive
 * SP_GAME_MODE_BACK - if the user clicked on back button
 * SP_GAME_MODE_NEXT - if the user clicked on next button  when it was active
 * SP_GAME_MODE_START - if the user clicked on start button  when it was active
 * SP_GAME_MODE_QUIT - if the user clicked on quit in the window
 * SP_GAME_MODE_NONE - otherwise
 */
SP_GAME_MODE_EVENT spGameModeWindowHandleEvent(SPGameModeWin* src, SDL_Event* event);

/*
 * Hides the src game mode window.
 *
 * @param src - a pointer to the game mode window to hide
 */
void spGameModeWindowHide(SPGameModeWin* src);

/*
 * Shows the src game mode window.
 *
 * @param src - a pointer to the game mode window to show
 */
void spGameModeWindowShow(SPGameModeWin* src);


#endif /* GRAPHICS_SPCHESSGAMEMODEWIN_H_ */
