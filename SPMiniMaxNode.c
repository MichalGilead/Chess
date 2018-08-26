/*
 * SPMiniMaxNode.c
 *
 *  Created on: 9 Jun 2017
 *      Author: Sivan
 */

#include "SPMiniMaxNode.h"


int suggestedMoveRec(node* n, unsigned int depth, unsigned int difficulty, int a, int b, bool maximizingPlayer, int currPlayer, int* mallocPtr) {
	SP_CHESS_GAME_MESSAGE message;
	SP_CHESS_GAME_STATUS status;
	SPChessPiece* arr;
	SPChessMove* tmpMove;
	int currValue; //the current value of the node to compare to its children scores
	int score; //the game score that a child returned
	bool firstValidMoveFound = false; //indicator for finding the first child that is a valid move
	status = spChessGameCheckCheckmateOrTie(n->gameStatus); //checks if there is a winner, and determines the game status
	if (status == SP_CHESS_GAME_STATUS_ALLOCATION_ERROR){
		*mallocPtr = 0;
		return 0;
	}
	if (status == SP_CHESS_GAME_BLACK_KING_CHECKMATE){ //game over- white player won
		if (currPlayer == SP_CHESS_GAME_WHITE_COLOR) //returns INT_MAX if currPlayer is white, INT_MIN o.w.
			return INT_MAX;
		else
			return INT_MIN;
	}
	if (status== SP_CHESS_GAME_WHITE_KING_CHECKMATE){ //game over- black player won
		if (currPlayer == SP_CHESS_GAME_WHITE_COLOR) //returns INT_MAX if currPlayer is black, INT_MIN o.w.
			return INT_MIN;
		else
			return INT_MAX;
	}

	if (status == SP_CHESS_GAME_TIE) //game over- tie
		return 0;

	//we have reached to maximal depth- the current node is a leaf
	//returns the score of current game represented by the node
	if (depth==0)
		return gameScore(n->gameStatus, currPlayer);

	//sets arr to point to the currentPlayer of the node's game pieces array
	arr= (n->gameStatus->currentPlayer==SP_CHESS_GAME_WHITE_COLOR)? n->gameStatus->currWhitePieces : n->gameStatus->currBlackPieces;
	if (maximizingPlayer){ //maximizing player- the current level is a max-level
		currValue = INT_MIN; //initializes game value to INT_MIN
		for (int k=0; k<SP_CHESS_GAME_SPAN*2; k++){
			if (arr[k].isAlive){ //goes over each alive piece
				for (int i=0; i<SP_CHESS_GAME_SPAN; i++) //goes over each position on board
					for (int j=0; j<SP_CHESS_GAME_SPAN; j++){
						tmpMove=createMove(arr[k].row, arr[k].column, i, j); //creates the move of the piece to the i,j position
						if (tmpMove==NULL){
							*mallocPtr = 0;
							return 0;
						}
						n->gameStatus->currentPlayer = currPlayer;
						//checks whether the move is valid
						if (spChessGameIsValidMove(n->gameStatus, tmpMove) == SP_CHESS_GAME_SUCCESS){
							if (!firstValidMoveFound && depth==difficulty){
								//first valid move was found- updates firstValidMoveFound and sets the node's move
								setMove(n->move, arr[k].row, arr[k].column, i, j);
								firstValidMoveFound = true;
							}

							message = spChessGameSetMove(n->gameStatus,tmpMove); //sets the move in the current game
							destroyMove(tmpMove);
							if (message == SP_CHESS_GAME_ALLOCATION_ERROR){ //changes the indicator to hold for malloc error
								*mallocPtr = 0;
								return 0;
							}

							//invokes suggestedMoveRec, decreasing the depth by one and changing to minimizing player
							//stores the score in score
							score = suggestedMoveRec(n, depth-1, difficulty, a, b, false, currPlayer, mallocPtr);
							n->gameStatus->currentPlayer = currPlayer; //changes back to maximizing player's turn for the undo
							spChessGameUndoPrevMove(n->gameStatus); //undo the last move

							if (*mallocPtr == 0){ //malloc error occurred somewhere in the recursion
								return 0; //terminates the current run
							}

							if(score > currValue){ //checks if score is higher than currValue
								currValue = score; //updates currValue to be the score that the child returned
								if (depth==difficulty) //if we reached the root- updates it's move to hold the better one
									setMove(n->move, arr[k].row, arr[k].column, i, j);
							}
							a = a>currValue? a:currValue; //updates a
							if (b <= a){ //pruning, cuts off this branch
								return currValue;;
							}
						}
						else{ //not a valid move
							destroyMove(tmpMove);
						}
					}
			}
		}
		return currValue; //returns game's value
	}

	else{ //minimizing player - the current level is a min-level
		currValue = INT_MAX; //initializes game value to INT_MAX
		for (int k=0; k<SP_CHESS_GAME_SPAN*2; k++){
			if (arr[k].isAlive){ //goes over each alive piece
				for (int i=0; i<SP_CHESS_GAME_SPAN; i++) //goes over each position on board
					for (int j=0; j<SP_CHESS_GAME_SPAN; j++){
						tmpMove=createMove(arr[k].row, arr[k].column, i, j); //creates the move of the piece to the i,j position
						if (tmpMove==NULL){
							*mallocPtr = 0;
							return 0;
						}
						n->gameStatus->currentPlayer = 1-currPlayer;
						if (spChessGameIsValidMove(n->gameStatus, tmpMove) == SP_CHESS_GAME_SUCCESS){ //checks whether the move is valid
							message = spChessGameSetMove(n->gameStatus,tmpMove); //sets the move in the current game
							destroyMove(tmpMove);
							if (message == SP_CHESS_GAME_ALLOCATION_ERROR){
								*mallocPtr = 0; //changes the indicator to hold 0 for malloc error
								return 0;
							}
							//invokes suggestedMoveRec, decreasing the depth by one and changing to maximizing player
							//stores the score in score
							score = suggestedMoveRec(n, depth-1, difficulty, a, b, true, currPlayer, mallocPtr);

							n->gameStatus->currentPlayer = 1-currPlayer; //changes back to minimizing player's turn for the undo
							spChessGameUndoPrevMove(n->gameStatus); //undo the last move
							if (*mallocPtr == 0){ //malloc error occurred somewhere in the recursion
								destroyMove(tmpMove);
								return 0; //terminates the current run
							}

							if(score <= currValue){ //checks if score is lower than currValue
								currValue = score; //updates currValue to be the score that the child returned
							}
							b = b<currValue? b:currValue; //updates b
							if (b <= a){ //pruning, cuts off this branch
								return currValue;
							}
						}
						else{ //not a valid move
							destroyMove(tmpMove);
						}
					}
			}
		}
		return currValue; //returns game's value
	}
}


int gameScore(SPChessGame* src, int currPlayer){
	int gameBoardWeight[SP_CHESS_GAME_SPAN][SP_CHESS_GAME_SPAN];
	int gameScore = 0; //holds the score of src's game board

	//invokes GameBoardWeightCalc to initialize gameBoardWeight to the desired signs
	gameBoardWeightCalc(gameBoardWeight, src->gameBoard, currPlayer);

	for (int i=0; i<SP_CHESS_GAME_SPAN;i++){ //calculates game's score according to the scoring function
		for (int j=0; j<SP_CHESS_GAME_SPAN;j++){
			if (src->gameBoard[i][j] == SP_CHESS_GAME_BLACK_PAWN || src->gameBoard[i][j] == SP_CHESS_GAME_WHITE_PAWN)
				gameScore+= 1*gameBoardWeight[i][j];
			else if (src->gameBoard[i][j] == SP_CHESS_GAME_BLACK_KNIGHT || src->gameBoard[i][j] == SP_CHESS_GAME_WHITE_KNIGHT)
				gameScore+= 3*gameBoardWeight[i][j];
			else if (src->gameBoard[i][j] == SP_CHESS_GAME_BLACK_BISHOP || src->gameBoard[i][j] == SP_CHESS_GAME_WHITE_BISHOP)
				gameScore+= 3*gameBoardWeight[i][j];
			else if (src->gameBoard[i][j] == SP_CHESS_GAME_BLACK_ROOK || src->gameBoard[i][j] == SP_CHESS_GAME_WHITE_ROOK)
				gameScore+= 5*gameBoardWeight[i][j];
			else if (src->gameBoard[i][j] == SP_CHESS_GAME_BLACK_QUEEN || src->gameBoard[i][j] == SP_CHESS_GAME_WHITE_QUEEN)
				gameScore+= 9*gameBoardWeight[i][j];
			else if (src->gameBoard[i][j] == SP_CHESS_GAME_BLACK_KING || src->gameBoard[i][j] == SP_CHESS_GAME_WHITE_KING)
				gameScore+= 100*gameBoardWeight[i][j];
		}
	}
	return gameScore;
}

void gameBoardWeightCalc(int gameBoardWeight[][SP_CHESS_GAME_SPAN], char gameBoard[][SP_CHESS_GAME_SPAN],char currPlayer){
	//goes over each cell in gameBoardWeight
	for (int i=0; i<SP_CHESS_GAME_SPAN;i++){
		for (int j=0; j<SP_CHESS_GAME_SPAN;j++){
			if (gameBoard[i][j] == SP_CHESS_GAME_EMPTY_ENTRY)
				gameBoardWeight[i][j] = 0; //set empty entry to 0
			if (isWhitePiece(gameBoard[i][j])){
				if (currPlayer == SP_CHESS_GAME_WHITE_COLOR)
					gameBoardWeight[i][j] = 1; //set current player's piece to 1 (positive sign)
				else
					gameBoardWeight[i][j] = -1; //set opponent player's piece to -1 (positive sign)
			}
			else if (isBlackPiece(gameBoard[i][j])){
				if (currPlayer == SP_CHESS_GAME_BLACK_COLOR)
					gameBoardWeight[i][j] = 1; //set current player's piece to 1 (positive sign)
				else
					gameBoardWeight[i][j] = -1; //set opponent player's piece to -1 (positive sign)
			}
		}
	}
}

node* createNode(SPChessGame* currGame){
	node* newNode = (node*)malloc(sizeof(node)); //creates new node
	if(newNode == NULL){ //checks if there was a memory allocation error
		return NULL;
	}

	newNode->gameStatus = spChessGameCopy(currGame); //copies the current game
	if (newNode->gameStatus == NULL){ //malloc error
		free(newNode);
		return NULL;
	}

	newNode->move = createMove(0,0,0,0); //creates move with zero's values
	if (newNode->move == NULL){ //malloc error
		spChessGameDestroy(newNode->gameStatus);
		free(newNode);
		return NULL;
	}
	return newNode;
}

void nodeDestroy(node* src){
	if (src!=NULL){
		spChessGameDestroy(src->gameStatus);
		destroyMove(src->move);
		free(src);
	}
}



