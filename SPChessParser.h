#ifndef SPFIARPARSER_H_
#define SPFIARPARSER_H_

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//specify the maximum line length
#define SP_MAX_LINE_LENGTH 1024


//a type used to represent a command
typedef enum {
	SP_GAME_MODE,//SETTING COMMANDS
	SP_DIFFICULTY,
	SP_USER_COLOR,
	SP_LOAD,
	SP_DEFAULT,
	SP_PRINT_SETTING,
	SP_START,
	SP_MOVE,//GAME STATE COMMANDS
	SP_GET_MOVES,
	SP_SAVE,
	SP_UNDO,
	SP_QUIT,
	SP_RESET,
	SP_INVALID_LINE,
	SP_ALLOCATION_ERROR,
} SP_COMMAND;

//a type used to represent move command args
typedef struct move_command{
	char srcRow[SP_MAX_LINE_LENGTH];
	char srcColumn[SP_MAX_LINE_LENGTH];
	char dstRow[SP_MAX_LINE_LENGTH];
	char dstColumn[SP_MAX_LINE_LENGTH];
} SPMoveCommand;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SP_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int intArg;
	char strArg[SP_MAX_LINE_LENGTH];
	SPMoveCommand* moveCommand;
} SPCommand;

/**
 * Creates a new command, allocates it's memory and initialize it's
 * fields
 *
 * @return
 * an SPcommand pointer or NULL if malloc has failed
 */
SPCommand* createCommand();

/**
 * Destroys a given command - frees it's memory and it's fields memory
 *
 */
void commandDestroy(SPCommand* src);

/**
 * Checks if a specified string represents a valid integer.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool spParserIsInt(const char* str);

/**
 * Checks if a specified string represents an upper case letter
 *
 * @return
 * true if the string represents an upper case letter, and false otherwise.
 */
bool spParserIsUpperCaseLetter(const char* str);


/**
 * Updates the SPMoveCommand fields if the command is move and the arguments are valid
 *
 * @arg
 *   command - the current command
 *   firstArg - a string of the first arg
 *   secondArg - a string of the second arg
 *   thirdArg - a string of the third arg
 *   wordsInLine - number of words in user's command
 *
 */
void updateMoveCommandArgs(SPCommand* command, char* firstArg, char* secondArg, char* thirdArg, int wordsInLine);


/**
 * Updates a command according to a parsed command and checks if it's valid
 *
 * @params
 * currCommand - a pointer to a command
 * commandName - the parsed comand name (first word)
 * firstArg - the first command argument
 * secondArg - the second command argument
 * thirdArg - the third comand argument
 * wordsInLine - number of words in command
 *
 */
void spGetCommand(SPCommand* currCommand, char* commandName, char* firstArg, char* secondArg, char* thirdArg, int wordsInLine);

/**
 * Parses a specified line and updates the command using spGetCommand
 *
 *@params
 * currCommand - a pointer to a command
 * str - the atring that the user entered
 *
 */
void spParserPraseLine(SPCommand* currCommand, const char* str);

/**
 * checks for a pointer memory allocation error
 *
 * @p - the pointer we want to check
 *
 * @return
 * true - if there was a memory allocation error
 * false - otherwise
 *
 */
bool parserCheckMallocError(void* p);

/**
 * frees the memory of any pointer (void*)
 *
 * @src - the pointer we want to free
 *
 */
void parserDestroy(void* src);


#endif

