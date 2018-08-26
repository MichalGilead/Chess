/*
 * SPChessGameWin.h
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#ifndef GRAPHICS_SPCHESSGAMEWIN_H_
#define GRAPHICS_SPCHESSGAMEWIN_H_

#include <SDL.h>
#include <SDL_video.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SPChessGame.h"
#include "SPMainAux.h"

#define SP_CHESS_HISTORY_SIZE 6
#define SP_CHESS_PIECES_SIZE 73

//game events
typedef enum {
	SP_GAME_BLACK_KING_CHECK,
	SP_GAME_WHITE_KING_CHECK,
	SP_GAME_BLACK_KING_CHECKMATE,
	SP_GAME_WHITE_KING_CHECKMATE,
	SP_GAME_TIE,
	SP_GAME_NOT_OVER,
	SP_GAME_INVALID_MOVE,
	SP_GAME_RESTART,
	SP_GAME_SAVE,
	SP_GAME_LOAD,
	SP_GAME_UNDO,
	SP_GAME_MAIN_MENU,
	SP_GAME_QUIT,
	SP_GAME_INVALID_ARGUMENT,
	SP_GAME_ALLOCATION_ERROR,
	SP_GAME_NONE
} SP_GAME_EVENT;

//all game window fields
typedef struct{
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	SDL_Texture* gameBoardPanelTexture;
	SDL_Texture* buttonsPanelTexture;
	SDL_Texture* restartTexture;
	SDL_Texture* saveTexture;
	SDL_Texture* loadTexture;
	SDL_Texture* inactiveUndoTexture;
	SDL_Texture* activeUndoTexture;
	SDL_Texture* mainMenuTexture;
	SDL_Texture* quitTexture;
	SDL_Texture* blackPawnTexture;
	SDL_Texture* blackBishopTexture;
	SDL_Texture* blackKnightTexture;
	SDL_Texture* blackRookTexture;
	SDL_Texture* blackQueenTexture;
	SDL_Texture* blackKingTexture;
	SDL_Texture* whitePawnTexture;
	SDL_Texture* whiteBishopTexture;
	SDL_Texture* whiteKnightTexture;
	SDL_Texture* whiteRookTexture;
	SDL_Texture* whiteQueenTexture;
	SDL_Texture* whiteKingTexture;
	SPChessGame* game;
	SPChessMove* dndMove; //drag and drop event
	//mouse coordinates during drag motion
	int motionX;
	int motionY;
	bool savedOnLastTurn;//indicates if game was saved after user's last turn or not - for notification before exit
}SPGameWin;

/*
 *Creates a new game window - allocates memory to all it's fields
 */
SPGameWin* spGameWindowCreate();

/*
 *Draws the game window
 *
 *@params
 *src - game window pointer
 */
void spGameWindowDraw(SPGameWin* src);

/*
 *Destroys the game window and all it's fields
 *
 *@params
 *src - game window pointer
 */
void spGameWindowDestroy(SPGameWin* src);

/*
 *Hides the game window
 *
 *@params
 *src - game window pointer
 */
void spGameWindowHide(SPGameWin* src);

/*
 *Shows the game window
 *
 *@params
 *src - game window pointer
 */
void spGameWindowShow(SPGameWin* src);

/*
 *handles a computer move
 *
 *@params
 *src - game window pointer
 *
 *@return
 *the game event after a computer move
 */
SP_GAME_EVENT handleComputerMove(SPGameWin* src);

/*
 *Draws the game window
 *
 *@params
 *src - game window pointer
 *event - an sdl event
 *
 *@return
 *the game event after a computer move
 */
SP_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src, SDL_Event* event);


#endif /* GRAPHICS_SPCHESSGAMEWIN_H_ */
