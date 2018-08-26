/*
 * SPChessGameWin.c
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#include "SPChessGameWin.h"

//Inner functions
int isClickOnRestart(int x, int y) {
	if ((x >= 15 && x <= 165) && (y >= 75 && y <= 125)) {
		return 1;
	}
	return 0;
}

int isClickOnSave(int x, int y) {
	if ((x >= 15 && x <= 165) && (y >= 140 && y <= 190)) {
		return 1;
	}
	return 0;
}

int isClickOnLoad(int x, int y) {
	if ((x >= 15 && x <= 165) && (y >= 205 && y <= 255)) {
		return 1;
	}
	return 0;
}

int isClickOnUndo(int x, int y) {
	if ((x >= 15 && x <= 165) && (y >= 270 && y <= 320)) {
		return 1;
	}
	return 0;
}

int isClickOnMainMenu(int x, int y) {
	if ((x >= 15 && x <= 165) && (y >= 460 && y <= 510)) {
		return 1;
	}
	return 0;
}

int isGameClickOnQuit(int x, int y) {
	if ((x >= 15 && x <= 165) && (y >= 525 && y <= 575)) {
		return 1;
	}
	return 0;
}

int isClickOnGameBoard (int x, int y){
	if ((x >= 200 && x <= 800) && (y >= 0 && y <= 600)) {
		return 1;
	}
	return 0;
}

//initializes a game texture
SDL_Texture* initiaizeGameTexture(SPGameWin* res, char* imageAddress, bool setColor){
	SDL_Texture* currentTexture;
	SDL_Surface* loadingSurface = NULL; //Used as temp surface
	loadingSurface = SDL_LoadBMP(imageAddress);
	if (loadingSurface == NULL ) {
		printf("ERROR: Could not create %s surface: %s\n", imageAddress, SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}

	if (setColor)//if should make background transparent
		SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 255, 255, 255));
	currentTexture = SDL_CreateTextureFromSurface(res->gameRenderer, loadingSurface);
	if (currentTexture == NULL ) {
		printf("ERROR: Could not create %s texture: %s\n", imageAddress, SDL_GetError());
		spGameWindowDestroy(res);
		SDL_FreeSurface(loadingSurface);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return currentTexture;
}

//initializes all game textures
bool initializeAllGameTextures(SPGameWin* res){
	if ((res->gameBoardPanelTexture = initiaizeGameTexture(res,	"./graphics/images/gameBoardPanel.bmp", false)) == NULL)
		return false;

	if ((res->buttonsPanelTexture = initiaizeGameTexture(res, "./graphics/images/buttonsPanel.bmp", false)) == NULL)
		return false;

	if (( res->restartTexture = initiaizeGameTexture(res, "./graphics/images/restartButton.bmp",false)) == NULL)
		return false;

	if ((res->saveTexture = initiaizeGameTexture(res, "./graphics/images/saveButton.bmp", false)) == NULL)
		return false;

	if ((res->loadTexture = initiaizeGameTexture(res, "./graphics/images/loadButton.bmp", false)) == NULL)
		return false;

	if ((res->inactiveUndoTexture = initiaizeGameTexture(res, "./graphics/images/inactiveUndoButton.bmp", false)) == NULL)
		return false;

	if ((res->activeUndoTexture = initiaizeGameTexture(res, "./graphics/images/activeUndoButton.bmp", false)) == NULL)
		return false;

	if ((res->mainMenuTexture = initiaizeGameTexture(res, "./graphics/images/mainMenuButton.bmp", false)) == NULL)
		return false;

	if ((res->quitTexture = initiaizeGameTexture(res, "./graphics/images/quitButton.bmp", false)) == NULL)
		return false;

	if ((res->blackPawnTexture = initiaizeGameTexture(res, "./graphics/images/blackPawn.bmp", true)) == NULL)
		return false;

	if ((res->blackBishopTexture = initiaizeGameTexture(res, "./graphics/images/blackBishop.bmp", true)) == NULL)
		return false;

	if ((res->blackKnightTexture = initiaizeGameTexture(res, "./graphics/images/blackKnight.bmp", true)) == NULL)
		return false;

	if ((res->blackRookTexture = initiaizeGameTexture(res, "./graphics/images/blackRook.bmp", true)) == NULL)
		return false;

	if ((res->blackQueenTexture = initiaizeGameTexture(res, "./graphics/images/blackQueen.bmp", true)) == NULL)
		return false;

	if ((res->blackKingTexture = initiaizeGameTexture(res, "./graphics/images/blackKing.bmp", true)) == NULL)
		return false;

	if ((res->whitePawnTexture = initiaizeGameTexture(res, "./graphics/images/whitePawn.bmp", true)) == NULL)
		return false;

	if ((res->whiteBishopTexture = initiaizeGameTexture(res, "./graphics/images/whiteBishop.bmp", true)) == NULL)
		return false;

	if ((res->whiteKnightTexture = initiaizeGameTexture(res, "./graphics/images/whiteKnight.bmp", true)) == NULL)
		return false;

	if ((res->whiteRookTexture = initiaizeGameTexture(res, "./graphics/images/whiteRook.bmp", true)) == NULL)
		return false;

	if ((res->whiteQueenTexture = initiaizeGameTexture(res, "./graphics/images/whiteQueen.bmp", true)) == NULL)
		return false;

	if ((res->whiteKingTexture = initiaizeGameTexture(res, "./graphics/images/whiteKing.bmp", true)) == NULL)
		return false;
	return true;
}

SPGameWin* spGameWindowCreate() {
	SPGameWin* res = (SPGameWin*) calloc(sizeof(SPGameWin), 1);
	if (res == NULL ) {//allocation error
		printf("ERROR: Couldn't create SPGameWin struct\n");
		return NULL ;
	}

	// Create an application window with the following settings:
	res->gameWindow = SDL_CreateWindow("Chess Game", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			800,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_HIDDEN
			);

	// Check that the window was successfully created
	if (res->gameWindow == NULL ) {
		// In the case that the window could not be made...
		printf("ERROR: Could not create game window: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	//create renderer
	res->gameRenderer = SDL_CreateRenderer(res->gameWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->gameRenderer == NULL ) {
		printf("ERROR: Could not create game renderer: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	//create new game
	res->game = spChessGameCreate(SP_CHESS_HISTORY_SIZE);
	if (res->game == NULL ) {//allocation error
		printf("ERROR: Couldn't copy game\n");
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->savedOnLastTurn = true;//set savedOnLastTurn to true

	if (!initializeAllGameTextures(res))//initialize all game textures and return null if allocation error occurred
		return NULL;

	return res;
}

void spGameWindowDestroy(SPGameWin* src) {
	if (src==NULL) {
		return;
	}
	//destroy game
	spChessGameDestroy(src->game);
	//destroy all textures
	if (src->whiteKingTexture != NULL ) {
		SDL_DestroyTexture(src->whiteKingTexture);
	}
	if (src->whiteQueenTexture != NULL ) {
		SDL_DestroyTexture(src->whiteQueenTexture);
	}
	if (src->whiteRookTexture != NULL ) {
		SDL_DestroyTexture(src->whiteRookTexture);
	}
	if (src->whiteKnightTexture != NULL ) {
		SDL_DestroyTexture(src->whiteKnightTexture);
	}
	if (src->whiteBishopTexture != NULL ) {
		SDL_DestroyTexture(src->whiteBishopTexture);
	}
	if (src->whitePawnTexture != NULL ) {
		SDL_DestroyTexture(src->whitePawnTexture);
	}
	if (src->blackKingTexture != NULL ) {
		SDL_DestroyTexture(src->blackKingTexture);
	}
	if (src->blackQueenTexture != NULL ) {
		SDL_DestroyTexture(src->blackQueenTexture);
	}
	if (src->blackRookTexture != NULL ) {
		SDL_DestroyTexture(src->blackRookTexture);
	}
	if (src->blackKnightTexture != NULL ) {
		SDL_DestroyTexture(src->blackKnightTexture);
	}
	if (src->blackBishopTexture != NULL ) {
		SDL_DestroyTexture(src->blackBishopTexture);
	}
	if (src->blackPawnTexture != NULL ) {
		SDL_DestroyTexture(src->blackPawnTexture);
	}
	if (src->quitTexture != NULL ) {
		SDL_DestroyTexture(src->quitTexture);
	}
	if (src->mainMenuTexture != NULL ) {
		SDL_DestroyTexture(src->mainMenuTexture);
	}
	if (src->inactiveUndoTexture != NULL ) {
		SDL_DestroyTexture(src->inactiveUndoTexture);
	}
	if (src->activeUndoTexture != NULL ) {
		SDL_DestroyTexture(src->activeUndoTexture);
	}
	if (src->loadTexture != NULL ) {
		SDL_DestroyTexture(src->loadTexture);
	}
	if (src->saveTexture != NULL ) {
		SDL_DestroyTexture(src->saveTexture);
	}
	if (src->restartTexture != NULL ) {
		SDL_DestroyTexture(src->restartTexture);
	}
	if (src->gameBoardPanelTexture != NULL ) {
		SDL_DestroyTexture(src->gameBoardPanelTexture);
	}
	if (src->buttonsPanelTexture != NULL ) {
			SDL_DestroyTexture(src->buttonsPanelTexture);
		}
	//destroy renderer
	if (src->gameRenderer != NULL ) {
		SDL_DestroyRenderer(src->gameRenderer);
	}
	//destroy window
	if (src->gameWindow != NULL ) {
		SDL_DestroyWindow(src->gameWindow);
	}
	free(src);
}

void spGameWindowDraw(SPGameWin* src) {
	if(src == NULL){
		return;
	}
	SDL_Rect gameBoardR = { .x = 200, .y = 0, .w = 600, .h = 600 };//game board rect
	SDL_Rect buttonsPanelR = { .x = 0, .y = 0, .w = 200, .h = 600 };//button panel rect
	SDL_Rect restartR = { .x = 15, .y = 75, .w = 150, .h = 50 };//restart button rect
	SDL_Rect saveR = { .x = 15, .y = 140, .w = 150, .h = 50 };//save button rect
	SDL_Rect loadR = { .x = 15, .y = 205, .w = 150, .h = 50 };//load button rect
	SDL_Rect undoR = { .x = 15, .y = 270, .w = 150, .h = 50 };//undo button rect
	SDL_Rect mainMenuR = { .x = 15, .y = 460, .w = 150, .h = 50 };//main button rect
	SDL_Rect quitR = { .x = 15, .y = 525, .w = 150, .h = 50 };//quit button rect
	SDL_Rect pieceR = { .x = 0, .y = 0, .w = SP_CHESS_PIECES_SIZE, .h = SP_CHESS_PIECES_SIZE };//initialize a chess piece rect

	SDL_SetRenderDrawColor(src->gameRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->gameRenderer);
	SDL_RenderCopy(src->gameRenderer, src->gameBoardPanelTexture, NULL, &gameBoardR);//put game board
	SDL_RenderCopy(src->gameRenderer, src->buttonsPanelTexture, NULL, &buttonsPanelR);//put buttons panel
	if (src->game->gameSettings->gameMode == SP_CHESS_GAME_1_PLAYER_MODE){//if 1 player mode
		//if computer started and history has at least three steps or if user started and history has at least two steps - put undo button
		if ((src->game->gameSettings->userColor != SP_CHESS_GAME_WHITE_COLOR && src->game->history->actualSize>=3) ||
				(src->game->gameSettings->userColor == SP_CHESS_GAME_WHITE_COLOR && src->game->history->actualSize>=2))
			SDL_RenderCopy(src->gameRenderer, src->activeUndoTexture, NULL, &undoR);
		else
			SDL_RenderCopy(src->gameRenderer, src->inactiveUndoTexture, NULL, &undoR);//put inactive undo button
	}
	SDL_RenderCopy(src->gameRenderer, src->saveTexture, NULL, &saveR);//put save button
	SDL_RenderCopy(src->gameRenderer, src->loadTexture, NULL, &loadR);//put load button
	SDL_RenderCopy(src->gameRenderer, src->restartTexture, NULL, &restartR);//put restart button
	SDL_RenderCopy(src->gameRenderer, src->mainMenuTexture, NULL, &mainMenuR);//put main menu button
	SDL_RenderCopy(src->gameRenderer, src->quitTexture, NULL, &quitR);//put quit button

	//for each position on game board put matching piece in position
	for (int i = 0; i < SP_CHESS_GAME_SPAN; i++) {
		for (int j = 0; j < SP_CHESS_GAME_SPAN; j++) {
			if (src->dndMove != NULL && src->dndMove->srcRow == i && src->dndMove->srcCol == j){//draw piece during a drag
				pieceR.x = src->motionX-SP_CHESS_PIECES_SIZE/2;
				pieceR.y = src->motionY-SP_CHESS_PIECES_SIZE/2;
			}
			else{//draw standing piece
				pieceR.x = 211 + (j* SP_CHESS_PIECES_SIZE);
				pieceR.y = 11 + (i * SP_CHESS_PIECES_SIZE);
			}
			if (src->game->gameBoard[i][j]==SP_CHESS_GAME_BLACK_PAWN) {
				SDL_RenderCopy(src->gameRenderer, src->blackPawnTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_WHITE_PAWN) {
				SDL_RenderCopy(src->gameRenderer, src->whitePawnTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_BLACK_BISHOP) {
				SDL_RenderCopy(src->gameRenderer, src->blackBishopTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_WHITE_BISHOP) {
				SDL_RenderCopy(src->gameRenderer, src->whiteBishopTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_BLACK_ROOK) {
				SDL_RenderCopy(src->gameRenderer, src->blackRookTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_WHITE_ROOK) {
				SDL_RenderCopy(src->gameRenderer, src->whiteRookTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_BLACK_KNIGHT) {
				SDL_RenderCopy(src->gameRenderer, src->blackKnightTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_WHITE_KNIGHT) {
				SDL_RenderCopy(src->gameRenderer, src->whiteKnightTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_BLACK_QUEEN) {
				SDL_RenderCopy(src->gameRenderer, src->blackQueenTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_WHITE_QUEEN) {
				SDL_RenderCopy(src->gameRenderer, src->whiteQueenTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_BLACK_KING) {
				SDL_RenderCopy(src->gameRenderer, src->blackKingTexture, NULL, &pieceR);
			} else if (src->game->gameBoard[i][j]==SP_CHESS_GAME_WHITE_KING) {
				SDL_RenderCopy(src->gameRenderer, src->whiteKingTexture, NULL, &pieceR);
			}
		}
	}
	SDL_RenderPresent(src->gameRenderer);//present window
}

SP_GAME_EVENT handleComputerMove(SPGameWin* src){
	SP_CHESS_GAME_STATUS status;
	//return game event matching the game status after computer turn
	if (computerMove(src->game)==0)
		return SP_GAME_ALLOCATION_ERROR;
	status = spChessGameCheckCheckmateOrTie(src->game);
	if (status == SP_CHESS_GAME_WHITE_KING_CHECK) {
		return SP_GAME_WHITE_KING_CHECK;
	} else if (status == SP_CHESS_GAME_WHITE_KING_CHECKMATE) {
		return SP_GAME_WHITE_KING_CHECKMATE;
	} else if (status==SP_CHESS_GAME_BLACK_KING_CHECK) {
		return SP_GAME_BLACK_KING_CHECK;
	} else if (status==SP_CHESS_GAME_BLACK_KING_CHECKMATE) {
		return SP_GAME_BLACK_KING_CHECKMATE;
	} else if (status==SP_CHESS_GAME_TIE) {
		return SP_GAME_TIE;
	} else{
		return SP_GAME_NOT_OVER;
	}

}


SP_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src, SDL_Event* event) {
	SP_CHESS_GAME_STATUS status;
	SP_CHESS_GAME_MESSAGE message;
	if (event == NULL || src == NULL ) {
		return SP_GAME_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONDOWN:
		if (isClickOnGameBoard(event->button.x, event->button.y)){//if clicked on game board - start dragging a piece
			src->dndMove = createMove(event->button.y / SP_CHESS_PIECES_SIZE, (event->button.x-200) / SP_CHESS_PIECES_SIZE, -1, -1);//NEED TO ADD DST
			if (src->dndMove == NULL)
				return SP_GAME_ALLOCATION_ERROR;
		}
		break;
	case SDL_MOUSEMOTION://save position of piece during drag
		src->motionX = event->motion.x;
		src->motionY = event->motion.y;
		break;
	case SDL_MOUSEBUTTONUP:
		if (isClickOnRestart(event->button.x, event->button.y)) {//if clicked on restart - restart game
			initializeGameBoard(src->game->gameBoard);
			initializePiecesArrays(src->game);
			src->game->currentPlayer = SP_CHESS_GAME_WHITE_COLOR;  //initializes currentPlayer
			spArrayListClear(src->game->history); //clear history
			return SP_GAME_RESTART;
		} else if (isClickOnSave(event->button.x, event->button.y)) {//if click on save - return save event
			return SP_GAME_SAVE;
		} else if (isClickOnLoad(event->button.x, event->button.y)) {//if click on load - return load event
			return SP_GAME_LOAD;
		//if click on undo and undo is active
		} else if (src->game->gameSettings->gameMode == SP_CHESS_GAME_1_PLAYER_MODE &&
				((src->game->gameSettings->userColor != SP_CHESS_GAME_WHITE_COLOR && src->game->history->actualSize>=3) ||
				(src->game->gameSettings->userColor == SP_CHESS_GAME_WHITE_COLOR && src->game->history->actualSize>=2))
				&&	isClickOnUndo(event->button.x, event->button.y)) {
			if (undoCommandInterperter(src->game)==0){//undo last move - quit if allocation error failed
				printf("ERROR: undo command failed");
				return SP_GAME_QUIT;
			}
			return SP_GAME_UNDO;
		} else if (isClickOnMainMenu(event->button.x, event->button.y)) {//if click on main menu - return main menu event
			return SP_GAME_MAIN_MENU;
		} else if (isGameClickOnQuit(event->button.x, event->button.y)) {//if click on quit - return quit event
			return SP_GAME_QUIT;
		} else if (src->dndMove != NULL && isClickOnGameBoard(event->button.x, event->button.y)){//if there was a piece drag
			//set move from drag start position to release position
			setMove (src->dndMove ,src->dndMove->srcRow, src->dndMove->srcCol, event->button.y / SP_CHESS_PIECES_SIZE, (event->button.x-200) / SP_CHESS_PIECES_SIZE);
			message = spChessGameSetMove(src->game, src->dndMove);//get set move message
			destroyMove(src->dndMove);
			src->dndMove = NULL;
			if (message == SP_CHESS_GAME_SUCCESS){
				status = spChessGameCheckCheckmateOrTie(src->game);
				if (status == SP_CHESS_GAME_WHITE_KING_CHECK) {
					return SP_GAME_WHITE_KING_CHECK;
				} else if (status == SP_CHESS_GAME_WHITE_KING_CHECKMATE) {
					return SP_GAME_WHITE_KING_CHECKMATE;
				} else if (status==SP_CHESS_GAME_BLACK_KING_CHECK) {
					return SP_GAME_BLACK_KING_CHECK;
				} else if (status==SP_CHESS_GAME_BLACK_KING_CHECKMATE) {
					return SP_GAME_BLACK_KING_CHECKMATE;
				} else if (status==SP_CHESS_GAME_TIE) {
					return SP_GAME_TIE;
				} else{
					return SP_GAME_NOT_OVER;
				}
			} else if (message == SP_CHESS_GAME_ALLOCATION_ERROR){
				return SP_GAME_ALLOCATION_ERROR;
			} else {
				return SP_GAME_INVALID_MOVE;
			}
		}
		destroyMove(src->dndMove);
		src->dndMove = NULL;
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {//if clicked the window's x button - return quit event
			return SP_GAME_QUIT;
		}
		break;
	default:
		return SP_GAME_NONE;
	}
	return SP_GAME_NONE;
}

void spGameWindowHide(SPGameWin* src){
	SDL_HideWindow(src->gameWindow);
}

void spGameWindowShow(SPGameWin* src){
	SDL_ShowWindow(src->gameWindow);
}
