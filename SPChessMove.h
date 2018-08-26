/*
 * SPChessMove.h
 *
 *  Created on: 16 Sep 2017
 *      Author: Sivan
 */

#ifndef SPCHESSMOVE_H_
#define SPCHESSMOVE_H_
#include <stdio.h>
#include <stdlib.h>

//a structure that represents a chess game move.
typedef struct sp_chess_move_ {
	int srcRow; //the source raw position
	int srcCol; //the source column position
	int dstRow; //the destination raw position
	int dstCol; //the destination column position
} SPChessMove;

/**
 * Creates a new chess game move.
 * @param srcRow - the source row of the move
 * @param srcCol - the source column of the move
 * @param dstRow - the destination row of the move
 * @param dstCol - the destination column of the move
 * @return
 *  NULL, if an allocation error occurred.
 * An instant of a chess game move otherwise.
 */
SPChessMove* createMove(int srcRow, int srcCol, int dstRow, int dstCol);

/**
 * Sets the given chess game move.
 * pre-condition : The function is called with the assertion that all arguments are valid.
 *
 * @param move - the move to set
 * @param srcRow - the source row of the move
 * @param srcCol - the source column of the move
 * @param dstRow - the destination row of the move
 * @param dstCol - the destination column of the move
 * @param pieceInDst - the opponent piece that was captured in move or an empty entry
 * @param check - the check state before move
 */
void setMove(SPChessMove* move, int srcRow, int srcCol, int dstRow, int dstCol);

/**
 * Creates an exact copy of a given chess game move.
 *
 * @param src - the source chess game move to copy.
 * @return
 * NULL if either an allocation error occurs or src == NULL.
 * A new copy of the source chess game move, otherwise.
 */
SPChessMove* copyMove(SPChessMove* src);

/**
 * frees the memory of a given chess game move. If the
 * source move is NULL, then the function does nothing.
 *
 * @src - the chess game move we want to free
 *
 */
void destroyMove(SPChessMove* move);

#endif /* SPCHESSMOVE_H_ */
