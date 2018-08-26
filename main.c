/* * main.c
 *
 *  Created on: 8 Jun 2017
 *      Author: Sivan
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_video.h>
#include "SPChessGUIManager.h"
#include "SPChessGame.h"
#include "SPChessParser.h"
#include "SPMiniMax.h"
#include "SPMainAux.h"

#define SP_CHESS_HISTORY_SIZE 6

int main(int argc, char** argv) {
	if (argc==2 && !strcmp(argv[1],"-g")){//run in GUI mode
		if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
			printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
			return 0;
		}
		SPGuiManager* manager = spManagerCreate();//initialize GUI manager
		if (manager == NULL ) {
			SDL_Quit();
			return 0;
		}
		SDL_Event event;
		while (1) {//while we don't quit the game - continue handling events
			SDL_WaitEvent(&event);
			if (spManagerHandleEvent(manager, &event) == SP_MANAGER_QUIT) {
				break;
			}
			spManagerDraw(manager);
		}
		spManagerDestroy(manager);//destroy manager after quit
		SDL_Quit();
		return 0;
	}

	//console mode
	int init;//initialization status indicator
	SP_CHESS_GAME_STATUS status;//gets game status - black/white check/checkmate / tie / not over
	int currTurn; //indication for the same turn of the user
	int innerIterCnt=1; //indication for first inner iteration in user's turn
	SPChessGame* currGame;//saves the game
	int commandInt; //helps determine how to continue the game after a user's command
	SPCommand* currCommand = createCommand();//saves user's commands

	if (checkMallocError(currCommand)){ //if there was an allocation error than terminates the game
		return 0;
	}

	//creates new game
	currGame = spChessGameCreate(SP_CHESS_HISTORY_SIZE);//creates new game
	if (checkMallocError(currGame)){//allocation error - terminates the game
		commandDestroy(currCommand);
		return 0;
	}

	//initialization
	init = gameInitialization(currGame);
	if (init==0){//allocation error during initialization - terminates the game
		spChessGameDestroy(currGame);
		commandDestroy(currCommand);
		return 0;
	}

	//each iteration represents a single turn
	while(true){
		status = spChessGameCheckCheckmateOrTie(currGame); //checks if there is a winner, and determines the game status
		if (status==SP_CHESS_GAME_STATUS_ALLOCATION_ERROR){//allocation error - terminates the game
			printf("ERROR: malloc has failed");
			spChessGameDestroy(currGame);
			commandDestroy(currCommand);
			return 0;
		}
		currTurn = 1;
		innerIterCnt = 1;//first iteration of the turn
		while (currTurn){ //while it is still the same turn
			commandInt = playGame(status, currCommand, currGame, innerIterCnt);//activates game moves
			if (commandInt==0){//game over, quit move in initialization or memory allocation error - terminates the game
				spChessGameDestroy(currGame);
				commandDestroy(currCommand);
				return 0;
			}
			if (commandInt==1){//moving to the next turn of the user
				currTurn=0;
			}
			innerIterCnt++;//moving to next iteration of the turn
		}
	}
	return 0;
}


