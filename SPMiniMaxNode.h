#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "limits.h"

#include "SPChessGame.h"

//a new type that is used to represent a node in the minimax tree.
//the node encapsulate a current game status
typedef struct node{
	SPChessMove* move; //the optimal move to suggest
	SPChessGame* gameStatus; //the current game

}node;

/**
 * "Building" recursively the miniMax tree, each call represents a node in the tree-
 * there is no creation of an actual tree structure.
 *
 * @param n - a node in the tree that represent a game status
 * @param depth - (difficulty - the level of the tree we are at)
 * @param difficulty - the difficulty level of the game and the maximal depth we will build the tree to
 * @param a - the alpha value of the node - the maximum score that the maximizing player is assured of
 * @param b - the beta value of the node - the minimum score that the minimizing player is assured of
 * @param maximizingPlayer - true if the node game's current player invoked that function, false o.w.
 * @param currPlayer - the player that invoked that function
 * @param mallocPtr - indicator for a malloc error
 * @return
 * The score of the current game(represented by node) yielded by an optimal move
 */
int suggestedMoveRec(node* n, unsigned int depth, unsigned int difficulty, int a, int b, bool maximizingPlayer, int currPlayer, int* mallocPtr);


/**
 * Calculates the score of a given game status by the score function:
 * pawn, knight, bishop, rook, queen and king gets 1, 3, 3, 5, 9, and 100 respectively,
 * where currPlayer gets a positive sign for its pieces values and the opponent player gets a negative one.
 * pre-condition: all the parameters are valid.
 *
 * @param src - the source game
 * @currPlayer - the player that invoked the suggestedMoveRec function
 *
 * @return
 * the game's score
 */
int gameScore(SPChessGame* src, int currPlayer);

/**
 * Fills a gameBoardWeight matrix of the same size as gameBoard with +1 for
 * each of the current player's pieces in gameBoard, -1 for each of the other player's
 * pieces, and 0 for each of the empty entries
 * pre-condition: all the parameters are valid.
 *
 * @gameBoardWeight - the matrix to fill
 * @gameBoard - the matrix representing the game board
 * @currPlayer - The current player (aka the player that invoked the suggestedMoveRec function)
 */
void gameBoardWeightCalc(int gameBoardWeight[][SP_CHESS_GAME_SPAN], char gameBoard[][SP_CHESS_GAME_SPAN],char currPlayer);

/**
 * Creates a new node with a given currGame (by coping it to the node's gameStatus field)
 * and a demo move (initialized to zero's).
 *
 * @param currGame - the source to copy to gameStatus field
 * @return
 * NULL, if an allocation error occurred.
 * An instant of a node otherwise.
 */
node* createNode(SPChessGame* currGame);

/**
 * frees all memory resources associated with the source node. If the
 * source node is NULL, then the function does nothing.
 *
 * @src - the node we want to free
 *
 */
void nodeDestroy(node* src);

#endif

