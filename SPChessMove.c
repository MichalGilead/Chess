/*
 * SPChessMove.c
 *
 *  Created on: 16 Sep 2017
 *      Author: Sivan
 */

#include "SPChessMove.h"

SPChessMove* createMove(int srcRow, int srcCol, int dstRow, int dstCol){
	SPChessMove* move = (SPChessMove*)malloc(sizeof(SPChessMove));
	if (move==NULL) //malloc error
		return NULL;

	//initialize fields according to the given parameters
	move->srcRow = srcRow;
	move->srcCol = srcCol;
	move->dstRow = dstRow;
	move->dstCol = dstCol;
	return move;
}

void setMove(SPChessMove* move, int srcRow, int srcCol, int dstRow, int dstCol){
	move->srcRow = srcRow;
	move->srcCol = srcCol;
	move->dstRow = dstRow;
	move->dstCol = dstCol;
}

SPChessMove* copyMove(SPChessMove* src){
	SPChessMove* copy = (SPChessMove*)malloc(sizeof(SPChessMove));
	if (copy==NULL || src==NULL)
		return NULL; //malloc error
	copy->srcRow = src->srcRow;
	copy->srcCol = src->srcCol;
	copy->dstRow = src->dstRow;
	copy->dstCol = src->dstCol;
	return copy;
}

void destroyMove(SPChessMove* move){
	if (move!=NULL)
		free(move);
}
