/*
 * SPChessInit.h
 *
 *  Created on: 27 Aug 2017
 *      Author: Sivan
 */

#ifndef SPCHESSINIT_H_
#define SPCHESSINIT_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//settings macros
#define SP_CHESS_GAME_BLACK_COLOR_STRING "black"
#define SP_CHESS_GAME_WHITE_COLOR_STRING "white"
#define SP_CHESS_GAME_BLACK_COLOR 0
#define SP_CHESS_GAME_WHITE_COLOR 1
#define SP_CHESS_GAME_1_PLAYER_MODE 1
#define SP_CHESS_GAME_2_PLAYER_MODE 2

typedef enum sp_chess_init_message_t {
	SP_CHESS_INIT_INVALID_MODE,
	SP_CHESS_INIT_INVALID_DIFFICULTY,
	SP_CHESS_INIT_INVALID_FILE,
	SP_CHESS_INIT_INVALID_ARGUMENT,
	SP_CHESS_INIT_SUCCESS,
	SP_CHESS_INIT_ALLOCATION_ERROR
} SP_CHESS_INIT_MESSAGE;

//a structure that represents a chess game settings.
typedef struct sp_chess_settings_t {
	int gameMode; //specify game mode - SP_CHESS_GAME_1_PLAYER_MODE or SP_CHESS_GAME_2_PLAYER_MODE
	int difficultyLevel; //specify difficulty level - an integer between 1-4
	int userColor; //specify user color - SP_CHESS_GAME_BLACK_COLOR or SP_CHESS_GAME_WHITE_COLOR
} SPChessSettings;

/**
 * Creates a new game settings, and sets them to default values: gameMode to SP_CHESS_GAME_1_PLAYER_MODE,
 * difficultyLevel to 2 and userColor to SP_CHESS_GAME_WHITE_COLOR.
 *
 * @return
 * NULL if either a memory allocation failure occurs.
 * Otherwise, a new game settings instant is returned.
 */
SPChessSettings* spChessGameCreateSettings();

/**
 * Sets a given game settings to default values: gameMode to SP_CHESS_GAME_1_PLAYER_MODE,
 * difficultyLevel to 2 and userColor to SP_CHESS_GAME_WHITE_COLOR.
 * pre-condition: the parameter is valid (non NULL)
 *
 * @param src - the given game to set
 */
void setToDefault(SPChessSettings* src);

/**
 * returns the game mode of a given game
 * pre-condition: the parameter is valid (non NULL)
 *
 * @param src - the given game
 * @return the game mode of a given game
 */
int getGameMode(SPChessSettings* src);

/**
 * returns the difficulty level of a given game
 * pre-condition: the parameter is valid (non NULL)
 *
 * @param src - the given game
 * @return the difficulty level of a given game
 */
int getDifficultyLevel(SPChessSettings* src);

/**
 * returns the user color of a given game
 * pre-condition: the parameter is valid (non NULL)
 *
 * @param src - the given game
 * @return the user color of a given game
 */
int getUserColor(SPChessSettings* src);

/**
 * Sets the game mode of a given game to the given game mode.
 * pre-condition: the parameters are valid- src isn't NULL and
 * game mode is SP_CHESS_GAME_1_PLAYER_MODE or SP_CHESS_GAME_2_PLAYER_MODE.
 *
 * @param src - the given game
 * @param gameMode - the given game mode
 */
void setGameMode(SPChessSettings* src, int gameMode);

/**
 * Sets the difficulty level of a given game to the given difficulty level.
 * pre-condition: the parameters are valid- src isn't NULL and
 * difficulty level is an integer between 1-4.
 *
 * @param src - the given game
 * @param gameMode - the given difficulty level
 */
void setDifficultyLevel(SPChessSettings* src, int difficultyLevel);

/**
 * Sets the user color of a given game to the given user color.
 * pre-condition: the parameters are valid- src isn't NULL and
 * user color is SP_CHESS_GAME_BLACK_COLOR or SP_CHESS_GAME_WHITE_COLOR.
 *
 * @param src - the given game
 * @param gameMode - the given user color
 */
void setUserColor(SPChessSettings* src, int userColor);

/**
 * On success, the function prints the game settings. If an error occurs, then the
 * function does nothing.
 *
 * @param src - the source game
 */
void spChessGamePrintSettings(SPChessSettings* src);

/**
 * frees the memory of a given game Settings. If the
 * source parameter is NULL, then the function does nothing.
 *
 * @src - the given game Settings we want to free
 *
 */
void spChessSettingsDestroy(SPChessSettings* src);

#endif /* SPCHESSINIT_H_ */
