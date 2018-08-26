/*
 * SPChessInit.c
 *
 *  Created on: 27 Aug 2017
 *      Author: Sivan
 */

#include "SPChessInit.h"


SPChessSettings* spChessGameCreateSettings(){
	SPChessSettings* newSettings = (SPChessSettings*)malloc(sizeof(SPChessSettings));
	if (newSettings == NULL){ //malloc error
		return NULL;
	}

	//sets settings to default values by invoking setToDefault
	setToDefault(newSettings);

	return newSettings;
}

void setToDefault(SPChessSettings* src){
	setGameMode(src, SP_CHESS_GAME_1_PLAYER_MODE);
	setDifficultyLevel(src, 2);
	setUserColor(src, SP_CHESS_GAME_WHITE_COLOR);
}

int getGameMode(SPChessSettings* src){
	return src->gameMode;
}

int getDifficultyLevel(SPChessSettings* src){
	return src->difficultyLevel;
}

int getUserColor(SPChessSettings* src){
	return src->userColor;
}

void setGameMode(SPChessSettings* src, int gameMode){
	src->gameMode = gameMode;
}

void setDifficultyLevel(SPChessSettings* src, int difficultyLevel){
	src->difficultyLevel = difficultyLevel;
}

void setUserColor(SPChessSettings* src, int userColor){
	src->userColor = userColor;
}

void spChessGamePrintSettings(SPChessSettings* src){
	if(src == NULL) //invalid argument
		return;
	if(src->gameMode == 2){ //2 players mode
		printf("SETTINGS:\n");
		printf("GAME_MODE: 2\n");
	}
	else{ //1 player mode
		printf("SETTINGS:\n");
		printf("GAME_MODE: 1\n");
		printf("DIFFICULTY_LVL: %d\n", getDifficultyLevel(src));
		if(src->userColor==SP_CHESS_GAME_WHITE_COLOR)
			printf("USER_CLR: WHITE\n");
		else
			printf("USER_CLR: BLACK\n");
	}

}

void spChessSettingsDestroy(SPChessSettings* src){
	if (src!=NULL)
		free(src);
}


