/*
 * SPMiniMax.c
 *
 *  Created on: 9 Jun 2017
 *      Author: Sivan
 */

#include "SPMiniMax.h"

SPChessMove* spMinimaxSuggestMove(SPChessGame* currGame){
	SPChessMove* move; //the move to suggest
	int* mallocPtr; //indicator for malloc error. holds 1 for no error, 0 o.w.
	node* root;
	int difficulty = getDifficultyLevel(currGame->gameSettings); //saves the depth for the minimax algorithm

	if (currGame == NULL || difficulty <= 0 ){ //checks if one of the parameters is invalid
		return NULL;
	}

	mallocPtr = (int*)malloc(sizeof(int)); //indicator for a malloc error
	if (mallocPtr==NULL){ //checks if there was a memory allocation error
		return NULL;
	}

	*mallocPtr = 1; //initializes mallocPtr to point to the value 1, no malloc error

	root = createNode(currGame); //root is a node that represents the current game

	if (root == NULL){ //checks if there was a memory allocation error
		free(mallocPtr);
		return NULL;
	}

	//invokes suggestedMoveRec to calculates the desired move
	//send root the be the root of the minimax tree, difficulty to be the depth
	// and difficulty level, INT_MIN and INT_MAX to be the initial a and b values respectively, true for maximizingPlayer
	// and currentPlayer to be the player for which we invoked suggestMove
	suggestedMoveRec(root, difficulty, difficulty, INT_MIN, INT_MAX, true, currGame->currentPlayer, mallocPtr);

	move = copyMove(root->move); //copies the result- the move to suggest

	free(mallocPtr); //frees mallocPtr
	nodeDestroy(root); //frees root

	if (mallocPtr == 0) //checks if there was a memory allocation error in the recursion
		return NULL;

	return move;//if there was a malloc error in copyMove - will return NULL, o.w the desired move
}


