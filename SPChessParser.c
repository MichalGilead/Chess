/*
 * SPFIARParser.c
 *
 *  Created on: 13 Jun 2017
 *      Author: Sivan
 */

#include "SPChessParser.h"

SPCommand* createCommand(){
	SPCommand* newCommand = (SPCommand*)malloc(sizeof(SPCommand));
	if (newCommand == NULL){//allocation error
		return NULL;
	}
	newCommand->moveCommand = (SPMoveCommand*)malloc(sizeof(SPMoveCommand));
	if (newCommand->moveCommand == NULL){//allocation error
		free(newCommand);
		return NULL;
	}
	newCommand->validArg = false;

	return newCommand;
}

void commandDestroy(SPCommand* src){
	if (src!=NULL){
		free(src->moveCommand);
		free(src);
	}
}

bool spParserIsInt(const char* str){
	int i=0;
	while (str[i]!='\0'){
		if (!isdigit(str[i]))//if there's a char in string that is not an integer and isn't a minus in beginning of number - return false
			if (i!=0 || str[0] != '-')
				return false;
		i++;
	}
	return true;
}

bool spParserIsUpperCaseLetter(const char* str){
	int i=0;
	while (str[i]!='\0'){//if string is longer than 1 char or the single char is not an upper case letter - return false
		if (!isupper(str[i]) || i > 0)
				return false;
		i++;
	}
	return true;
}

void updateMoveCommandArgs(SPCommand* command, char* firstArg, char* secondArg, char* thirdArg, int wordsInLine){
	int i=1, j=0;
	char srcRowArg[SP_MAX_LINE_LENGTH], srcColArg[SP_MAX_LINE_LENGTH], dstRowArg[SP_MAX_LINE_LENGTH], dstColArg[SP_MAX_LINE_LENGTH];

	if (wordsInLine != 4 || strcmp(secondArg,"to"))//if there's no "to" or number of words in argument isn't 4 - command is invalid - return
		return;
	if (firstArg[0] == '<'){//if first arg begin with '<'
		while (firstArg[i]!=','){
			if (firstArg[i]!='\0'){//save string to srcRow
				srcRowArg[j]=firstArg[i];
				i++,j++;
			}
			else//if first arg doesn't contain a ',' - command is invalid - return
				return;
		}
		srcRowArg[j]='\0';
		i++;
		j=0;
		while (firstArg[i]!='>'){
			if (firstArg[i]!='\0'){
				srcColArg[j]=firstArg[i];//save string to srcCol
				i++,j++;
			}
			else//if first arg doesn't end with '>' - command is invalid - return
				return;
		}
		srcColArg[j]='\0';
		i=1, j=0;
		if (thirdArg[0] == '<'){//if third arg begin with '<'
			while (thirdArg[i]!=','){
				if (thirdArg[i]!='\0'){
					dstRowArg[j]=thirdArg[i];//save string to dstRow
					i++,j++;
				}
				else//if third arg doesn't contain a ',' - command is invalid - return
					return;
			}
			dstRowArg[j]='\0';
			i++;
			j=0;
			while (thirdArg[i]!='>'){
				if (thirdArg[i]!='\0'){
					dstColArg[j]=thirdArg[i];//save string to dstCol
					i++,j++;
				}
				else//if third arg doesn't end with '>' - command is invalid - return
					return;
			}
			dstColArg[j]='\0';
		}

		command->validArg = true;
		strcpy(command->moveCommand->srcRow, srcRowArg);
		strcpy(command->moveCommand->srcColumn, srcColArg);
		strcpy(command->moveCommand->dstRow, dstRowArg);
		strcpy(command->moveCommand->dstColumn, dstColArg);
	}
}

/**
 * Updates the intArg or strArg field if it exists and is int or string accordingly
 *
 * @arg
 *   command - pointer to current command
 *   wordsInLine - number of words in command
 *   arg - the argument string
 *   isInt - set to true if argument should be int and false if argument should be string
 */
void updateArg(SPCommand* command, int wordsInLine, char* arg,  bool isInt){
	if (wordsInLine==2 && isInt && spParserIsInt(arg)){ //if there is 1 argument and if it is integer - updates intArg
		command->validArg = true;
		command->intArg = atoi(arg);
	}
	else if (wordsInLine==2 && !isInt){//if there's 1 argument that shouldn't be an integer - updates straRG
		command->validArg = true;
		strcpy(command->strArg, arg);
	}
}

void spGetCommand(SPCommand* currCommand, char* commandName, char* firstArg, char* secondArg, char* thirdArg, int wordsInLine){
	currCommand->validArg = false; //initializing validArg to be false (i.e. invalid)
	//the command is "game_mode"
	if(!strcmp(commandName,"game_mode")&&wordsInLine==2){
		currCommand->cmd = SP_GAME_MODE;
		updateArg(currCommand, wordsInLine, firstArg, true);
	}
	//the command is "difficulty"
	else if(!strcmp(commandName,"difficulty")&&wordsInLine==2){
		currCommand->cmd = SP_DIFFICULTY;
		updateArg(currCommand, wordsInLine, firstArg, true);
	}
	//the command is "user_color"
	else if(!strcmp(commandName,"user_color")&&wordsInLine==2){
		currCommand->cmd = SP_USER_COLOR;
		updateArg(currCommand, wordsInLine, firstArg, true);
	}
	//the command is "load"
	else if(!strcmp(commandName,"load")&&wordsInLine==2){
		currCommand->cmd = SP_LOAD;
		updateArg(currCommand, wordsInLine, firstArg, false);
	}
	//the command is "default"
	else if(!strcmp(commandName,"default")&&wordsInLine==1){
		currCommand->cmd = SP_DEFAULT;
	}
	//the command is "print_setting"
	else if(!strcmp(commandName,"print_setting")&&wordsInLine==1){
		currCommand->cmd = SP_PRINT_SETTING;
	}
	//the command is "quit"
	else if(!strcmp(commandName,"quit")&&wordsInLine==1){
		currCommand->cmd = SP_QUIT;
	}
	//the command is "start"
	else if(!strcmp(commandName,"start")&&wordsInLine==1){
		currCommand->cmd = SP_START;
	}
	//the command is "move"
	else if(!strcmp(commandName,"move")&&wordsInLine==4){
		currCommand->cmd = SP_MOVE;
		updateMoveCommandArgs(currCommand, firstArg, secondArg, thirdArg, wordsInLine);

	}
	//the command is "save"
	else if(!strcmp(commandName,"save")&&wordsInLine==2){
		currCommand->cmd = SP_SAVE;
		updateArg(currCommand, wordsInLine, firstArg, false);
	}
	//the command is "undo"
	else if(!strcmp(commandName,"undo")&&wordsInLine==1){
		currCommand->cmd = SP_UNDO;
	}
	//the command is "reset"
	else if(!strcmp(commandName,"reset")&&wordsInLine==1){
		currCommand->cmd = SP_RESET;
	}
	//the string was one or more invalid word - isn't one of the operations above
	else{
		currCommand->cmd = SP_INVALID_LINE;
	}
}

void spParserPraseLine(SPCommand* currCommand, const char* str){
	char* tmpStr;
	char* parsedStrCmd; //pointer for the first string in the parsed string (for use of strtok command)
	char* parsedStrFirstArg; //pointer for the first argument
	char* parsedStrSecondArg = ""; //pointer for the second argument
	char* parsedStrThirdArg = ""; //pointer for the third argument
	char* strCopy;

	int cnt=0; //counter for number of separated strings entered by the user
	strCopy = (char*)malloc(SP_MAX_LINE_LENGTH*sizeof(char)); //copy of str (parameter)
	if (parserCheckMallocError(strCopy)){ //checks if there was an allocation error
		currCommand->cmd = SP_ALLOCATION_ERROR;
		return;
	}
	strcpy(strCopy,str); //Copies str to strCopy
	tmpStr = strtok(strCopy, " \t\r\n"); //parses strCpy and stores the result pointer in parsedStrCmd

	while (tmpStr!=NULL){ //stores the parsed string in parsedStrCmd
		cnt++;
		if (cnt == 1)
			parsedStrCmd = tmpStr;
		if (cnt == 2)
			parsedStrFirstArg = tmpStr;
		if (cnt == 3)
			parsedStrSecondArg = tmpStr;
		if (cnt == 4)
			parsedStrThirdArg = tmpStr;
		tmpStr = strtok(NULL, " \t\r\n"); //advances parsedStr to point to the next string
	}

	spGetCommand(currCommand, parsedStrCmd, parsedStrFirstArg, parsedStrSecondArg, parsedStrThirdArg, cnt);//update command according to parsed string
	parserDestroy(strCopy);
}

bool parserCheckMallocError(void* p){
	if (p == NULL){
		printf("Error: malloc has failed");
		return true;
	}

	return false;
}

void parserDestroy(void* src){
	if (src!=NULL)
		free(src);
}

