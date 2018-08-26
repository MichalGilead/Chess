#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SPChessGame.h"
#include "SPChessParser.h"
#include "SPMiniMax.h"
#include "SPChessInit.h"

/**
 * checks for a pointer memory allocation error
 *
 * @p - the pointer we want to check
 *
 * @return
 * 1 - if there was a memory allocation error
 * 0 - otherwise
 */
int checkMallocError(void* p);

/**
 * resets a given game by reinitializing it's board and it's pieces arrays,
 * setting the current player to white, clearing it's history, and
 * set it's settings to default
 *
 * @src - a pointer that gets the game to reset
 *
 */
void gameReset(SPChessGame* src);

/**
 * initializes the game settings according to the user's commands
 *
 * @currGame - a pointer to the current game
 *
 * @return
 * 0 - if there was a malloc error or 'quit' command
 * 1 - otherwise
 *
 */
int gameInitialization(SPChessGame* currGame);

/**
 * Gets the computer move from minimax algorithm and sets it to game
 *
 * @currGame - the current game
 *
 * @return
 * 0 - if there was an allocation error
 * 1 - otherwise
 *
 */
int computerMove(SPChessGame* currGame);

/**
 * handles an undo command
 *
 * @currGame - the current game
 *
 * @return
 * 0 - if there was an allocation error
 * -1 - if command was invalid - empty history or 2 players mode
 * 1 - otherwise
 *
 */
int undoCommandInterperter(SPChessGame* currGame);

/**
 * Interprets and takes the user's command to action, and prints relevant errors\messages
 *
 * @currCommand - a pointer that gets the user's commands
 * @currGame - the current game
 *
 * @return
 * -1 - if it's still the same turn
 * 0 - if there was a malloc error or 'quit' command
 * 1 - if the current turn of the user is done (valid command was acted)
 *
 */
int commandInterperter(SPCommand* currCommand, SPChessGame* currGame);

/**
 * checks the game status and prints a message accordingly, than calls commandsInterperter
 *
 * @status - contains the current game check/checkmate/tie status
 * @currCommand - a pointer to the user's commands
 * @currGame -  a pointer to the current game
 * @innerItterCnt - an int indicator for first inner iteration in user's turn
 *
 * @return
 * -1 - if it's still the same turn
 * 0 - if there was a malloc error or 'quit' command
 * 1 - if the current turn of the user is done (valid command was acted)
 *
 */
int playGame(SP_CHESS_GAME_STATUS status, SPCommand* currCommand, SPChessGame* currGame, int innerIterCnt);

#endif
