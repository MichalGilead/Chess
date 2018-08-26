#ifndef SPMINIMAX_H_
#define SPMINIMAX_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "SPChessGame.h"
#include "SPMiniMaxNode.h"

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm up to a
 * specified length given in difficultyLevel field of the game settings. The current game state
 * doesn't change by this function including the history of previous moves.
 * pre-condition: the currentGame is not in game over state
 *
 * @param currentGame - The current game state
 * @return
 * NULL if either currentGame is NULL or memory allocation error occurred.
 * On success the function returns a SPChessMove instant
 * which is the best move for the current player.
 */
SPChessMove* spMinimaxSuggestMove(SPChessGame* currentGame);


#endif
