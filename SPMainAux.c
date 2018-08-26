/*
 * SPMainAux.c
 *
 *  Created on: 18 Jun 2017
 *      Author: Sivan
 */

#include "SPMainAux.h"

/**
 * checks if an int argument is 1 or 2
 *
 * @arg - an int
 *
 * @return
 * 1 - if the number is 1 or 2
 * 0 - otherwise
 */
int checkValidMode(int mode){
	return (mode==SP_CHESS_GAME_1_PLAYER_MODE || mode==SP_CHESS_GAME_2_PLAYER_MODE);
}
/**
 * checks if an int argument is between 1-4
 *
 * @arg - an int
 *
 * @return
 * 1 - if the number is between 1-4
 * 0 - otherwise
 */
int checkValidDifficulty(int difficulty){
	return (difficulty>=1 && difficulty<=4);
}
/**
 * checks if an int argument is 0 or 1
 *
 * @arg - an int
 *
 * @return
 * 1 - if the number is 0 or 1
 * 0 - otherwise
 */
int checkValidUserColor(int color){
	return (color==0 || color==1);
}
/**
 * checks if a string argument is an integer
 *
 * @arg - a string
 *
 * @return
 * 1 - if the string is an integer
 * 0 - otherwise
 */
int checkValidMoveRow(char* row){
	return (spParserIsInt(row));
}
/**
 * checks if a string argument is an upper case letter
 *
 * @arg - a string
 *
 * @return
 * 1 - if the string is an upper case letter
 * 0 - otherwise
 */
int checkValidMoveCol(char* col){
	return (spParserIsUpperCaseLetter(col));
}

int checkMallocError(void* p){
	if (p == NULL){
		printf("ERROR: malloc has failed");
		return 1;
	}
	return 0;
}
/**
 * frees the memory of any pointer (void*)
 *
 * @src - the pointer we want to free
 *
 */
void destroy(void* src){
	if (src!=NULL)
		free(src);
}

void gameReset(SPChessGame* src){
	initializeGameBoard(src->gameBoard); //initializes gameBoard
	initializePiecesArrays(src); //initializes pieces arrays
	src->currentPlayer = SP_CHESS_GAME_WHITE_COLOR;  //initializes currentPlayer
	spArrayListClear(src->history); //clears history
	setToDefault(src->gameSettings); //sets settings to default
}

/**
 * initializes the game settings according to the user's commands -
 * for both 1 player and 2 players modes
 *
 * @pre-condition: the command is valid
 *
 * @currGame - a pointer to the current game
 * @currCommand - a poointer to the current command
 *
 * @return
 * 0 - if there was a malloc error
 * 1 - otherwise
 *
 */
int initializationGeneralCommands(SPChessGame* currGame, SPCommand* currCommand){
	SP_CHESS_INIT_MESSAGE message;
	if (currCommand->cmd == SP_GAME_MODE){//if command is game mode
		if (checkValidMode(currCommand->intArg)){//sets game mode if arg is valid and print accordingly
			setGameMode(currGame->gameSettings, currCommand->intArg);
			if (currCommand->intArg == SP_CHESS_GAME_1_PLAYER_MODE)
				printf("Game mode is set to 1 player\n");
			else
				printf("Game mode is set to 2 players\n");
		}
		else
			printf("Wrong game mode\n");
	}

	else if (currCommand->cmd == SP_LOAD){//if command is load
		message = spChessGameLoadFromFile(currGame, currCommand->strArg);//load game from given file
		if (message == SP_CHESS_INIT_INVALID_FILE)//if given file path is not valid - prints message
			printf("Error: File doesn\'t exist or cannot be opened\n");
		else if (message == SP_CHESS_INIT_ALLOCATION_ERROR){//if allocation error occurred - return 0
			printf("ERROR: malloc has failed\n");
			return 0;
		}
	}

	else if (currCommand->cmd == SP_DEFAULT){//if command is default - set all settings to default
		setToDefault(currGame->gameSettings);
	}
	else if (currCommand->cmd == SP_PRINT_SETTING){//if command is print setting - prints the current game settings
		spChessGamePrintSettings(currGame->gameSettings);
	}

	return 1;
}

int gameInitialization(SPChessGame* currGame){
	char settingsArg[SP_MAX_LINE_LENGTH];//saves the user command
	SPCommand* currCommand = createCommand();//creates a command struct
	if (currCommand == NULL){//allocation error - return 0
		printf("ERROR: malloc has failed\n");
		return 0;
	}
	printf("Specify game setting or type \'start\' to begin a game with the current setting:\n");
	do{
		fgets(settingsArg, SP_MAX_LINE_LENGTH, stdin); //gets command from user
		spParserPraseLine(currCommand, settingsArg); //parses the command
		if (currCommand->cmd == SP_INVALID_LINE || currCommand->cmd == SP_MOVE || currCommand->cmd == SP_GET_MOVES
				|| currCommand->cmd == SP_SAVE || currCommand->cmd == SP_UNDO || currCommand->cmd == SP_RESET) //if the command is invalid- prints an error message
			printf("ERROR: invalid command\n");
		else if (getGameMode(currGame->gameSettings) == SP_CHESS_GAME_2_PLAYER_MODE){//2 players mode
			if (currCommand->cmd == SP_DIFFICULTY||currCommand->cmd == SP_USER_COLOR)//prints errors for invalid commands
				printf("ERROR: invalid command\n");
			else{
				if (!initializationGeneralCommands(currGame, currCommand)){//handles the user valid command
					commandDestroy(currCommand);
					return 0;
				}
			}
		}
		else{ //1 players mode
			if (currCommand->cmd == SP_DIFFICULTY){//if command is difficulty
				if (checkValidDifficulty(currCommand->intArg)){//sets difficulty level if arg is valid
					setDifficultyLevel(currGame->gameSettings, currCommand->intArg);
				}
				else{//prints message if arg is not valid
					if (currCommand->intArg == 5)
						printf("Expert level not supported, please choose a value between 1 to 4:\n");
					else
						printf("Wrong difficulty level. The value should be between 1 to 5\n");
				}
			}
			else if (currCommand->cmd == SP_USER_COLOR){//if command is user color
				if(checkValidUserColor(currCommand->intArg))//sets user color level if arg is valid
					setUserColor(currGame->gameSettings, currCommand->intArg);
				else//prints message if arg is not valid
					printf("ERROR: illegal color, please choose between 0 or 1\n");

			}
			else//if command is other than difficulty/user color
				if (!initializationGeneralCommands(currGame, currCommand)){
					commandDestroy(currCommand);
					return 0;
				}
		}
	}
	while(currCommand->cmd != SP_QUIT && currCommand->cmd != SP_START); //continue while the user has'nt entered a valid number or "quit"

	if (currCommand->cmd == SP_QUIT){
		printf("Exiting...\n");
		commandDestroy(currCommand);
		return 0;
	}
	commandDestroy(currCommand);
	return 1;
}

/**
 * returns the matching piece name (string) to a given piece char
 *
 * @c - a char
 *
 * @return
 * the matching piece name (string) to a given piece char or a
 * string with an empty entry sign if given char is not of a valid piece
 *
 */
char* spChessPieceCharToStr(char c){
	if (c==SP_CHESS_GAME_WHITE_PAWN || c==SP_CHESS_GAME_BLACK_PAWN)
		return SP_CHESS_GAME_PAWN_STR;
	if (c==SP_CHESS_GAME_WHITE_BISHOP || c==SP_CHESS_GAME_BLACK_BISHOP)
		return SP_CHESS_GAME_BISHOP_STR;
	if (c==SP_CHESS_GAME_WHITE_ROOK || c==SP_CHESS_GAME_BLACK_ROOK)
		return SP_CHESS_GAME_ROOK_STR;
	if (c==SP_CHESS_GAME_WHITE_KNIGHT || c==SP_CHESS_GAME_BLACK_KNIGHT)
		return SP_CHESS_GAME_KNIGHT_STR;
	if (c==SP_CHESS_GAME_WHITE_QUEEN || c==SP_CHESS_GAME_BLACK_QUEEN)
		return SP_CHESS_GAME_QUEEN_STR;
	if (c==SP_CHESS_GAME_WHITE_KING || c==SP_CHESS_GAME_BLACK_KING)
		return SP_CHESS_GAME_KING_STR;
	return "_";
}

int computerMove(SPChessGame* currGame){
	SPChessMove* compMove;
	compMove = spMinimaxSuggestMove(currGame);//gets the computer move from minimax algorithm
	if (compMove == NULL){ //memory allocation error - return 0
		printf("ERROR: malloc has failed\n");
		return 0;
	}

	spChessGameSetMove(currGame, compMove);//set the computer move in game
	//print the computer move
	printf("Computer: move %s at <%d,%c> to <%d,%c>\n", spChessPieceCharToStr(currGame->gameBoard[compMove->dstRow][compMove->dstCol]),
			SP_CHESS_GAME_SPAN-compMove->srcRow, compMove->srcCol+SP_CHESS_GAME_A_ASCII_VALUE,
			SP_CHESS_GAME_SPAN-compMove->dstRow, compMove->dstCol+SP_CHESS_GAME_A_ASCII_VALUE);
	destroyMove(compMove);
	return 1;
}

/**
 * handles a move command
 *
 * @currGame - the current game
 * @currCommand - the current command
 *
 * @return
 * 0 - if there was an allocation error
 * -1 - if command was invalid
 * 1 - otherwise
 *
 */
int moveCommandInterperter(SPCommand* currCommand, SPChessGame* currGame){
	SP_CHESS_GAME_MESSAGE currMessage;
	SPChessMove* move;
	if(!(currCommand->validArg)){//if arguments aren't valid - prints error
		printf("ERROR: invalid command\n");
		return -1;//stays in the same turn
	}
	//if the argument strings are not int for row and upper case letter for column print invalid position
	if (!checkValidMoveRow(currCommand->moveCommand->srcRow) || !checkValidMoveCol(currCommand->moveCommand->srcColumn)
			|| !checkValidMoveRow(currCommand->moveCommand->dstRow) || !checkValidMoveCol(currCommand->moveCommand->dstColumn)){
		printf("Invalid position on the board\n");
		return -1;//stays in the same turn
	}
	//create move from command args
	move = createMove(SP_CHESS_GAME_SPAN-atoi(currCommand->moveCommand->srcRow), *(currCommand->moveCommand->srcColumn)-SP_CHESS_GAME_A_ASCII_VALUE,
			SP_CHESS_GAME_SPAN-atoi(currCommand->moveCommand->dstRow), *(currCommand->moveCommand->dstColumn)-SP_CHESS_GAME_A_ASCII_VALUE);

	if (move == NULL){//allocation error - return 0
		printf("ERROR: malloc has failed\n");
		return 0;
	}
	currMessage = spChessGameSetMove(currGame, move);//sets the move if it's valid
	destroyMove(move);
	//if an allocation error occurred
	if (currMessage == SP_CHESS_GAME_ALLOCATION_ERROR){
		printf("ERROR: malloc has failed\n");
		return 0;//terminates
	}
	//if arguments position is out of range - prints error
	if (currMessage == SP_CHESS_GAME_INVALID_POSITION){
		printf("Invalid position on the board\n");
		return -1;//stays in the same turn
	}
	//if the source position doesn't contain a player's pawn - prints error
	if (currMessage == SP_CHESS_GAME_INVALID_PIECE){
		printf("The specified position does not contain your piece\n");
		return -1;//stays in the same turn
	}
	//if the source position doesn't contain a player's pawn - prints error
	if (currMessage == SP_CHESS_GAME_ILLEGAL_MOVE){
		printf("Illegal move\n");
		return -1;//stays in the same turn
	}
	return 1;//moves to next turn because a move was made
}

int undoCommandInterperter(SPChessGame* currGame){
	SPArrayListElement* computerMove;
	SPArrayListElement* userMove;
	SP_CHESS_GAME_MESSAGE message;
	if (spArrayListSize(currGame->history)<=1){//if history doesn't exist prints message
		printf("Empty history, move cannot be undone\n");
		return -1;//stays in the same turn
	}
	computerMove = spArrayListCopyElement(spArrayListGetLast(currGame->history)); //saves last move as computer's move
	if (checkMallocError(computerMove))//allocation error - return 0
		return 0;

	currGame->currentPlayer = 1 - currGame->currentPlayer; //changing back to computer color for doing first undo
	message = spChessGameUndoPrevMove(currGame);//undoes last(computer) move
	if (message == SP_CHESS_GAME_INVALID_ARGUMENT){//if the sent game was null
		listElementDestroy(computerMove);
		return 0;//terminates game
	}
	if (message == SP_CHESS_GAME_2_PLAYERS_MODE){//if there's no history due to 2 players mode, doesn't undo and prints message
		printf("Undo command not available in 2 players mode\n");
		listElementDestroy(computerMove);
		return -1;//stays in the same turn
	}
	//prints computer undo move
	printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n", currGame->currentPlayer == SP_CHESS_GAME_BLACK_COLOR ?
			SP_CHESS_GAME_WHITE_COLOR_STRING : SP_CHESS_GAME_BLACK_COLOR_STRING, SP_CHESS_GAME_SPAN-computerMove->move->dstRow, computerMove->move->dstCol+SP_CHESS_GAME_A_ASCII_VALUE,
			SP_CHESS_GAME_SPAN-computerMove->move->srcRow, computerMove->move->srcCol+SP_CHESS_GAME_A_ASCII_VALUE);
	userMove = spArrayListCopyElement(spArrayListGetLast(currGame->history));//saves last move as user's move
	if (checkMallocError(userMove)){//allocation error - return 0
		listElementDestroy(computerMove);
		return 0;
	}
	message = spChessGameUndoPrevMove(currGame);//undoes last(user) move
	currGame->currentPlayer = 1 - currGame->currentPlayer; //changing back to user color
	//prints user undo move
	printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n", currGame->currentPlayer == SP_CHESS_GAME_BLACK_COLOR ?
			SP_CHESS_GAME_BLACK_COLOR_STRING : SP_CHESS_GAME_WHITE_COLOR_STRING, SP_CHESS_GAME_SPAN-userMove->move->dstRow, userMove->move->dstCol+SP_CHESS_GAME_A_ASCII_VALUE,
			SP_CHESS_GAME_SPAN-userMove->move->srcRow, userMove->move->srcCol+SP_CHESS_GAME_A_ASCII_VALUE);
	listElementDestroy(computerMove);
	listElementDestroy(userMove);
	return 1;//moves to next turn because moves were undone
}

int commandInterperter(SPCommand* currCommand, SPChessGame* currGame){
	SP_CHESS_GAME_MESSAGE message;
	//if an allocation error occurred
	if (currCommand->cmd == SP_ALLOCATION_ERROR){
		printf("ERROR: malloc has failed");
		return 0;//terminates
	}
	//if the command is invalid - prints error
	if (currCommand->cmd == SP_INVALID_LINE){
		printf("ERROR: invalid command\n");
		return -1;//stays in the same turn
	}
	//if the command is move
	if (currCommand->cmd == SP_MOVE){
		return moveCommandInterperter(currCommand, currGame);//return -1 if invalid, 0 to terminate and 1 to continue to next turn
	}
	//if the command is save
	if (currCommand->cmd == SP_SAVE){
		message = spChessGameSaveToFile(currGame, currCommand->strArg);//saves current game to given file
		if (message==SP_CHESS_GAME_INVALID_ARGUMENT)//if currGame is null terminates
			return 0;
		if (message==SP_CHESS_GAME_INVALID_FILE)//if given file path isn't valid prints message
			printf("File cannot be created or modified\n");
		return -1;//stays in same turn
	}
	//if the command is undo
	if (currCommand->cmd == SP_UNDO){
		return undoCommandInterperter(currGame);//return -1 if invalid, 0 to terminate and 1 to continue to next turn
	}
	//if the command is reset
	if (currCommand->cmd == SP_RESET){
		gameReset(currGame);//reset game
		printf("Restarting...\n");
		if (!gameInitialization(currGame))//go to settings initialization
			return 0; //allocation error or "quit" command - terminates the game
		return 1;//moves to next turn because game restarted and we need to print the gameBoard
	}
	//if the command is quit - prints message and terminates the game
	if (currCommand->cmd == SP_QUIT){
		printf("Exiting...\n");
		return 0;
	}
	return -1;//stays in the same turn - none of the above commands was acted
}

int playGame(SP_CHESS_GAME_STATUS status, SPCommand* currCommand, SPChessGame* currGame, int innerIterCnt){
	char userCommand[SP_MAX_LINE_LENGTH];//initialized to read the user's command

	if (status== SP_CHESS_GAME_BLACK_KING_CHECK || status== SP_CHESS_GAME_WHITE_KING_CHECK){//if black or white king is threatened
		if (innerIterCnt == 1){//if it's the first iteration of a turn - prints message
			if (currGame->gameSettings->gameMode == SP_CHESS_GAME_1_PLAYER_MODE && currGame->currentPlayer == currGame->gameSettings->userColor)
				printf("Check!\n");
			else
				printf("Check: %s King is threatened!\n", status == SP_CHESS_GAME_BLACK_KING_CHECK? SP_CHESS_GAME_BLACK_COLOR_STRING : SP_CHESS_GAME_WHITE_COLOR_STRING);
		}
	}
	else if (status==SP_CHESS_GAME_TIE){//if there's a tie
		if (currGame->gameSettings->gameMode == SP_CHESS_GAME_1_PLAYER_MODE && currGame->currentPlayer == currGame->gameSettings->userColor)
			printf("The game ends in a tie\n");
		else
			printf("The game is tied\n");
		return 0;
	}
	//if black or white king is threatened and there are no legal moves (checkmate) - terminates
	else if (status==SP_CHESS_GAME_BLACK_KING_CHECKMATE || status==SP_CHESS_GAME_WHITE_KING_CHECKMATE){
		printf("Checkmate! %s player wins the game\n", status == SP_CHESS_GAME_BLACK_KING_CHECKMATE? SP_CHESS_GAME_WHITE_COLOR_STRING : SP_CHESS_GAME_BLACK_COLOR_STRING);
		return 0;
	}
	//if it's the computer turn - do computer move
	if (currGame->gameSettings->gameMode == SP_CHESS_GAME_1_PLAYER_MODE && currGame->currentPlayer != currGame->gameSettings->userColor){
		return computerMove(currGame);
	}
	else{
		//user's turn- prints board in first iteration
		if (innerIterCnt == 1)
			spChessGamePrintBoard(currGame); //prints the game board
		printf("%s player - enter your move:\n", currGame->currentPlayer == SP_CHESS_GAME_WHITE_COLOR ? SP_CHESS_GAME_WHITE_COLOR_STRING : SP_CHESS_GAME_BLACK_COLOR_STRING);
		fgets(userCommand,SP_MAX_LINE_LENGTH,stdin);//reads user's command
		spParserPraseLine(currCommand, userCommand);//parses user's command

		return commandInterperter(currCommand, currGame);//returns commandInterperter
	}
}

