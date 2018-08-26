/*
 * SPChessGUIManager.h
 *
 *  Created on: 25 בספט׳ 2017
 *      Author: michal
 */

#ifndef GRAPHICS_SPCHESSGUIMANAGER_H_
#define GRAPHICS_SPCHESSGUIMANAGER_H_


#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPChessGame.h"
#include "SPMainAux.h"
#include "SPChessGameWin.h"
#include "SPChessMainWin.h"
#include "SPChessDifficultyLevelWin.h"
#include "SPChessGameModeWin.h"
#include "SPChessUserColorWin.h"
#include "SPChessLoadWin.h"

#define SP_CHESS_HISTORY_SIZE 6

//indicator for current active window
typedef enum{
	SP_MAIN_WINDOW_ACTIVE,
	SP_GAME_WINDOW_ACTIVE,
	SP_LOAD_WINDOW_ACTIVE,
	SP_GAME_MODE_WINDOW_ACTIVE,
	SP_DIFFICULTY_LEVEL_WINDOW_ACTIVE,
	SP_USER_COLOR_WINDOW_ACTIVE
}ACTIVE_WINDOW;

//manager events
typedef enum{
	SP_MANAGER_QUIT,
	SP_MANAGER_NONE,
}SP_MANAGER_EVENET;

//a struct that saves all windows and the active window
typedef struct {
	SPMainWin* mainWin;
	SPGameModeWin* gameModeWin;
	SPDifficultyLevelWin* difficultyLevelWin;
	SPUserColorWin* userColorWin;
	SPLoadWin* loadWin;
	SPGameWin* gameWin;
	ACTIVE_WINDOW activeWin;
	bool loadFromGame;//indicates if we got to load window from game or feom main menu
} SPGuiManager;

/*
 * Creates the manger and all of it's windows and fields
 */
SPGuiManager* spManagerCreate();

/*
 * Destroys the manager and all windows - frees all memory
 *
 * @param
 * src - a pointer to the gui manager
 *
 */
void spManagerDestroy(SPGuiManager* src);

/*
 * Draws the current active window
 *
 * @param
 * src - a pointer to the gui manager
 */
void spManagerDraw(SPGuiManager* src);

/*
 * Draws the current active window
 *
 * @params
 * src - a pointer to the gui manager
 * event - a pointer to an sdl event
 *
 */
SP_MANAGER_EVENET spManagerHandleEvent(SPGuiManager* src, SDL_Event* event) ;



#endif /* GRAPHICS_SPCHESSGUIMANAGER_H_ */
