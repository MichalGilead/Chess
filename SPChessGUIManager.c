/*
 * SPChessGUIManager.c
 *
 *  Created on: 25 בספט׳ 2017
 *      Author: michal
 */


#include "SPChessGUIManager.h"

SPGuiManager* spManagerCreate() {
	SPGuiManager* res = (SPGuiManager*) calloc(sizeof(SPGuiManager), 1);
	if (res == NULL ) {
		return NULL ;
	}
	res->mainWin = spMainWindowCreate();//create main window
	if (res->mainWin == NULL ) {
		spManagerDestroy(res);
		printf("ERROR: Couldn't create main menu window\n");
		return NULL ;
	}
	res->gameWin = spGameWindowCreate();//create game window
	if (res->gameWin == NULL ) {
		spManagerDestroy(res);
		printf("ERROR: Couldn't create game window\n");
		return NULL;
	}
	res->loadWin = spLoadWindowCreate();//create load window
	if (res->loadWin == NULL ) {
		spManagerDestroy(res);
		printf("ERROR: Couldn't create load window\n");
		return NULL;
	}

	res->gameModeWin = spGameModeWindowCreate();//create game mode window
	if (res->gameModeWin == NULL ) {
		spManagerDestroy(res);
		printf("ERROR: Couldn't create game mode window\n");
		return NULL;
	}

	res->difficultyLevelWin = spDifficultyLevelWindowCreate();//create difficulty level window
	if (res->difficultyLevelWin == NULL ) {
		spManagerDestroy(res);
		printf("ERROR: Couldn't create difficulty level window\n");
		return NULL;
	}
	res->userColorWin = spUserColorWindowCreate();//create user color window
	if (res->userColorWin == NULL ) {
		spManagerDestroy(res);
		printf("ERROR: Couldn't create user color window\n");
		return NULL;
	}

	res->activeWin = SP_MAIN_WINDOW_ACTIVE;//set active window to main window
	res->loadFromGame = false;

	return res;
}

void spManagerDestroy(SPGuiManager* src) {
	if (src==NULL) {
		return;
	}
	spGameWindowDestroy(src->gameWin);//destroy game window

	spGameModeWindowDestroy(src->gameModeWin);//destroy game mode window

	spDifficultyLevelWindowDestroy(src->difficultyLevelWin);//destroy difficulty level color window

	spUserColorWindowDestroy(src->userColorWin);//destroy user color window

	spLoadWindowDestroy(src->loadWin);//destroy load window

	spMainWindowDestroy(src->mainWin);//destroy main window

	free(src); //free manager
}
void spManagerDraw(SPGuiManager* src) {
	if (!src) {
		return;
	}
	//draw the current active window
	if (src->activeWin == SP_MAIN_WINDOW_ACTIVE) {
		spMainWindowDraw(src->mainWin);
	} else if (src->activeWin == SP_GAME_MODE_WINDOW_ACTIVE){
		spGameModeWindowDraw(src->gameModeWin);
	} else if (src->activeWin == SP_DIFFICULTY_LEVEL_WINDOW_ACTIVE){
		spDifficultyLevelWindowDraw(src->difficultyLevelWin);
	} else if (src->activeWin == SP_USER_COLOR_WINDOW_ACTIVE){
		spUserColorWindowDraw(src->userColorWin);
	} else if (src->activeWin == SP_LOAD_WINDOW_ACTIVE){
		spLoadWindowDraw(src->loadWin);
	} else if (src->activeWin == SP_GAME_WINDOW_ACTIVE){
		spGameWindowDraw(src->gameWin);
	}
}

/*
 * Saves the current game to the available slot and moves all other slots to the next one.
 * If all slots are filled eliminates the last slot
 *
 * @param
 * src - a pointer to the manager
 *
 * @return
 * 0 - if an allocation error occurred
 * 1 - otherwise
 */
int spManagerSaveGameToSlot(SPGuiManager* src){
	SPChessGame* tmpGame;
	tmpGame = spChessGameCreate(SP_CHESS_HISTORY_SIZE);//create tmp game
	if (tmpGame == NULL){//allocation error - return 0
		printf("ERROR: could not create tmpGame\n");
		return 0;
	}
	if (src->loadWin->currSlotsNum < SLOTS_NUMBER)//update number of available saved games slots
		src->loadWin->currSlotsNum++;
	for (int i=src->loadWin->currSlotsNum; i>1; i--){//for each slot with saved game - pass saved game to next slot
		gameReset(tmpGame);
		spChessGameLoadFromFile(tmpGame, src->loadWin->slotsArr[i-2]->slotName);
		spChessGameSaveToFile(tmpGame, src->loadWin->slotsArr[i-1]->slotName);
	}
	spChessGameDestroy(tmpGame);
	spChessGameSaveToFile(src->gameWin->game, src->loadWin->slotsArr[0]->slotName);//save the current game to the first slot
	src->gameWin->savedOnLastTurn = true;//update indicator to true - saves after last turn
	return 1;
}


/*
 * Creates a message box to offer the user to save game before exit.
 *
 * @return - the user choice
 * -1 - if box couldn't be created
 * 0 - if the user don't want to save
 * 1 - is the user want to save
 * 2 - if the user cancelled the command
 */
int createSaveBeforeExitMessageBox(){
	const SDL_MessageBoxButtonData buttons[] = {
			{ 0, 0, "no" },
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
			{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
	};
	const SDL_MessageBoxColorScheme colorScheme = {
			{   { 255,   0,   0 },
					{   0, 255,   0 },
					{ 255, 255,   0 },
					{   0,   0, 255 },
					{ 255,   0, 255 }
			}
	};
	const SDL_MessageBoxData messageboxdata = {SDL_MESSAGEBOX_INFORMATION, NULL, "Save Before Exit",
			"Would you like to save the current game before exit?", SDL_arraysize(buttons), buttons, &colorScheme };
	int buttonId;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonId) < 0) {
		printf("ERROR: could not display message box");
		return -1;
	}
	return buttonId;
}

/*
 * Handles main window events
 *
 *@params
 * src - a pointer to the manager
 * event - the main event
 *
 * @return
 * SP_MANAGER_QUIT - if program is terminated
 * SP_MANAGER_NONE - if event doesn't terminate the program
 */
SP_MANAGER_EVENET handleManagerDueToMainEvent(SPGuiManager* src,
		SP_MAIN_EVENT event) {
	if (src == NULL ) {
		return SP_MANAGER_QUIT;
	}
	if (event == SP_MAIN_INVALID_ARGUMENT) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_MAIN_NEW_GAME) {//if starts a new game - moves to game mode window
		spMainWindowHide(src->mainWin);
		spGameModeWindowShow(src->gameModeWin);
		src->activeWin = SP_GAME_MODE_WINDOW_ACTIVE;
	}
	if (event == SP_MAIN_LOAD) {//if load was chosen - sets loadFromGame to false and moves to load window
		src->loadFromGame = false;
		spMainWindowHide(src->mainWin);
		spLoadWindowShow(src->loadWin);
		src->activeWin = SP_LOAD_WINDOW_ACTIVE;
	}
	if (event == SP_MAIN_QUIT) {//if quit was chosen - terminates program
		return SP_MANAGER_QUIT;
	}
	return SP_MANAGER_NONE;//in any other case - does nothing
}

/*
 * Handles game mode window events
 *
 *@params
 * src - a pointer to the manager
 * event - the game mode event
 *
 * @return
 * SP_MANAGER_QUIT - if program is terminated
 * SP_MANAGER_NONE - if event doesn't terminate the program
 */
SP_MANAGER_EVENET handleManagerDueToGameModeEvent(SPGuiManager* src,
		SP_GAME_MODE_EVENT event) {
	if (src == NULL ) {
		return SP_MANAGER_QUIT;
	}
	if (event == SP_GAME_MODE_INVALID_ARGUMENT) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_GAME_MODE_ONE_PLAYER) {//if event is 1 player mode - updates game setting to 1 player mode
		src->gameWin->game->gameSettings->gameMode = SP_CHESS_GAME_1_PLAYER_MODE;
	}
	if (event == SP_GAME_MODE_TWO_PLAYER) {//if event is 2 players mode - updates game setting to 2 players mode
		src->gameWin->game->gameSettings->gameMode = SP_CHESS_GAME_2_PLAYER_MODE;
	}
	if (event == SP_GAME_MODE_BACK){//if user wants to go back - goes back to main window
		spGameModeWindowHide(src->gameModeWin);
		spMainWindowShow(src->mainWin);
		src->activeWin = SP_MAIN_WINDOW_ACTIVE;
	}
	if (event == SP_GAME_MODE_NEXT){//if event is next - moves to difficulty level window
		spGameModeWindowHide(src->gameModeWin);
		spDifficultyLevelWindowShow(src->difficultyLevelWin);
		src->activeWin = SP_DIFFICULTY_LEVEL_WINDOW_ACTIVE;
	}
	if (event == SP_GAME_MODE_START){//if event is start - goes to game window
		spGameModeWindowHide(src->gameModeWin);
		spGameWindowShow(src->gameWin);
		src->activeWin = SP_GAME_WINDOW_ACTIVE;
	}
	if (event == SP_GAME_MODE_QUIT) {//if event is quit - terminates the program
		return SP_MANAGER_QUIT;
	}
	return SP_MANAGER_NONE;//in any other case does nothing

}

/*
 * Handles difficulty level window events
 *
 *@params
 * src - a pointer to the manager
 * event - the difficulty level event
 *
 * @return
 * SP_MANAGER_QUIT - if program is terminated
 * SP_MANAGER_NONE - if event doesn't terminate the program
 */
SP_MANAGER_EVENET handleManagerDueToDifficultyLevelEvent(SPGuiManager* src,
		SP_DIFFICULTY_LEVEL_EVENT event) {
	if (src == NULL ) {
		return SP_MANAGER_QUIT;
	}
	if (event == SP_DIFFICULTY_LEVEL_INVALID_ARGUMENT) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_DIFFICULTY_LEVEL_NOOB) {//if event is noob - sets difficulty level to 1
		src->gameWin->game->gameSettings->difficultyLevel = 1;
	}
	if (event == SP_DIFFICULTY_LEVEL_EASY) {//if event is easy - sets difficulty level to 2
		src->gameWin->game->gameSettings->difficultyLevel = 2;
	}
	if (event == SP_DIFFICULTY_LEVEL_MODERATE){//if event is moderate - sets difficulty level to 3
		src->gameWin->game->gameSettings->difficultyLevel = 3;
	}
	if (event == SP_DIFFICULTY_LEVEL_HARD){//if event is hard - sets difficulty level to 4
		src->gameWin->game->gameSettings->difficultyLevel = 4;
	}
	if (event == SP_DIFFICULTY_LEVEL_BACK){//if event is back - goes back to game mode window
		spDifficultyLevelWindowHide(src->difficultyLevelWin);
		spGameModeWindowShow(src->gameModeWin);
		src->activeWin = SP_GAME_MODE_WINDOW_ACTIVE;
	}
	if (event == SP_DIFFICULTY_LEVEL_NEXT){//if event is next - goes to user color window
		spDifficultyLevelWindowHide(src->difficultyLevelWin);
		spUserColorWindowShow(src->userColorWin);
		src->activeWin = SP_USER_COLOR_WINDOW_ACTIVE;
	}
	if (event == SP_DIFFICULTY_LEVEL_QUIT) {//if event is quit - terminates program
		return SP_MANAGER_QUIT;
	}
	return SP_MANAGER_NONE;

}

/*
 * Handles game window move events
 *
 *@params
 * src - a pointer to the manager
 * event - the game event
 *
 * @return
 * -1 - if invalid move was set
 * 0 - if game is over
 * 1 - otherwise
 */
int handleManagerDueToGameMoveEvent(SPGuiManager* src,
		SP_GAME_EVENT event){
	spManagerDraw(src);
	if (event == SP_GAME_INVALID_MOVE){//if event is invalid move return -1
		return -1;
	}
	else if (event == SP_GAME_BLACK_KING_CHECK) {//if event is black king check - open message box to inform and return 1
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Check!", "Black king is threatened", NULL );
	}
	else if (event == SP_GAME_BLACK_KING_CHECKMATE) {//if event is black king checkmate - open message box to inform and return 0
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Checkmate!", "White player wins the game", NULL );
		return 0;
	}
	else if (event == SP_GAME_WHITE_KING_CHECK) {//if event is white king check - open message box to inform and return 1
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Check!", "White king is threatened", NULL );
	}
	else if (event == SP_GAME_WHITE_KING_CHECKMATE) {//if event is white king checkmate - open message box to inform and return 0
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Checkmate!", "Black player wins the game", NULL );
		return 0;
	}
	else if (event == SP_GAME_TIE) {//if event is tie - open message box to inform and return 0
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game over", "The game is tied", NULL );
		return 0;
	}
	return 1;
}

/*
 * Handles a computer move
 *
 *@params
 * src - a pointer to the manager
 *
 * @return
 * 0 - if allocation error occurred
 * 1 - otherwise
 */
int spManagerHandleComputerMove(SPGuiManager* src){
	SP_GAME_EVENT computerEvent;
	computerEvent = handleComputerMove(src->gameWin);
	if (computerEvent == SP_GAME_ALLOCATION_ERROR)//allocation error - return 0
		return 0;
	if (handleManagerDueToGameMoveEvent(src, computerEvent)==0){//if event ended game - reset game and go to main menu window
		gameReset(src->gameWin->game);
		spGameWindowHide(src->gameWin);
		spMainWindowShow(src->mainWin);
		src->activeWin = SP_MAIN_WINDOW_ACTIVE;
	}
	return 1;
}

/*
 * Handles user color window events
 *
 *@params
 * src - a pointer to the manager
 * event - the user color event
 *
 * @return
 * SP_MANAGER_QUIT - if program is terminated
 * SP_MANAGER_NONE - if event doesn't terminate the program
 */
SP_MANAGER_EVENET handleManagerDueToUserColorEvent(SPGuiManager* src,
		SP_USER_COLOR_EVENT event) {
	SP_GAME_EVENT computerEvent;
	if (src == NULL ) {
		return SP_MANAGER_QUIT;
	}
	if (event == SP_USER_COLOR_INVALID_ARGUMENT) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_USER_COLOR_WHITE) {//if event is white color - set user color to white
		src->gameWin->game->gameSettings->userColor = SP_CHESS_GAME_WHITE_COLOR;
	}
	if (event == SP_USER_COLOR_BLACK) {//if event is black color - set user color to black
		src->gameWin->game->gameSettings->userColor = SP_CHESS_GAME_BLACK_COLOR;
	}
	if (event == SP_USER_COLOR_BACK){//if event is back - go back to difficulty level window
		spUserColorWindowHide(src->userColorWin);
		spDifficultyLevelWindowShow(src->difficultyLevelWin);
		src->activeWin = SP_DIFFICULTY_LEVEL_WINDOW_ACTIVE;
	}
	if (event == SP_USER_COLOR_START){//if event is start - move to game window
		spUserColorWindowHide(src->userColorWin);
		spGameWindowShow(src->gameWin);
		src->activeWin = SP_GAME_WINDOW_ACTIVE;
		//if first turn is computer's turn - play turn
		if (src->gameWin->game->currentPlayer != src->gameWin->game->gameSettings->userColor){
			computerEvent = handleComputerMove(src->gameWin);
			if (computerEvent == SP_GAME_ALLOCATION_ERROR){//allocation error - quit
				printf("ERROR: malloc has failed");
				return SP_MANAGER_QUIT;
			}
			handleManagerDueToGameMoveEvent(src, computerEvent);
		}
	}
	if (event == SP_USER_COLOR_QUIT) {//if event is quit - terminates program
		return SP_MANAGER_QUIT;
	}
	return SP_MANAGER_NONE;
}

/*
 * Handles load window events
 *
 *@params
 * src - a pointer to the manager
 * event - the load event
 *
 * @return
 * SP_MANAGER_QUIT - if program is terminated
 * SP_MANAGER_NONE - if event doesn't terminate the program
 */
SP_MANAGER_EVENET handleManagerDueToLoadEvent(SPGuiManager* src,
		SP_LOAD_EVENT event) {
	if (src == NULL ) {
		return SP_MANAGER_QUIT;
	}
	if (event == SP_LOAD_INVALID_ARGUMENT) {
		return SP_MANAGER_NONE;
	}
	if (event == SP_LOAD_LOAD) {//if event is load - loads the saved file to game and goes to game window
		spLoadWindowHide(src->loadWin);
		if (src->loadFromGame)//if arrived from game window - resets the game
			gameReset(src->gameWin->game);
		spChessGameLoadFromFile(src->gameWin->game, src->loadWin->slotsArr[src->loadWin->activeSlotIndex]->slotName);
		src->loadWin->activeSlotIndex = -1;//reset load fields
		src->loadWin->loadActive = false;
		spGameWindowShow(src->gameWin);
		src->activeWin = SP_GAME_WINDOW_ACTIVE;
	}
	if (event == SP_LOAD_BACK) {//if event is back - if loadFromGame is false goes back to main window, otherwise goes back to game window
		src->loadWin->activeSlotIndex = -1;//reset load fields
		src->loadWin->loadActive = false;
		spLoadWindowHide(src->loadWin);
		if (src->loadFromGame){
			spGameWindowShow(src->gameWin);
			src->activeWin = SP_GAME_WINDOW_ACTIVE;

		}else{
			spMainWindowShow(src->mainWin);
			src->activeWin = SP_MAIN_WINDOW_ACTIVE;
		}
	}
	if (event == SP_LOAD_QUIT) {//if event is quit - terminates the program
		return SP_MANAGER_QUIT;
	}
	return SP_MANAGER_NONE;
}

/*
 * Handles game window events
 *
 *@params
 * src - a pointer to the manager
 * event - the game event
 *
 * @return
 * SP_MANAGER_QUIT - if program is terminated
 * SP_MANAGER_NONE - if event doesn't terminate the program
 */
SP_MANAGER_EVENET handleManagerDueToGameEvent(SPGuiManager* src, SP_GAME_EVENT event) {
	int saveBeforeExitButton;
	int statusAfterMove;
	if (src == NULL ) {
		return SP_MANAGER_QUIT;
	}
	if (event == SP_GAME_ALLOCATION_ERROR){//allocation error - terminates program
		printf("ERROR: malloc has failed");
		return SP_MANAGER_QUIT;
	}
	if (event == SP_GAME_SAVE) {//if event is save - save current game to slot 1
		if (spManagerSaveGameToSlot(src)==0)
			return SP_MANAGER_QUIT;
	}
	else if (event == SP_GAME_LOAD) {//if event is load - go to load window and set loadFromGame to true
		src->loadFromGame = true;
		spGameWindowHide(src->gameWin);
		spLoadWindowShow(src->loadWin);
		src->activeWin = SP_LOAD_WINDOW_ACTIVE;
	}
	else if (event == SP_GAME_MAIN_MENU) {//leave game and go to main menu
		if (!src->gameWin->savedOnLastTurn){//if game is not saved - open message box
			saveBeforeExitButton = createSaveBeforeExitMessageBox();
			if (saveBeforeExitButton==-1){//error creating message box - exit program
				return SP_MANAGER_QUIT;
			}
			else if (saveBeforeExitButton==1){//game is saved - if failed to save - exit program
				if (spManagerSaveGameToSlot(src)==0)
					return SP_MANAGER_QUIT;
			}
			else if (saveBeforeExitButton==2)//cancel - stay in game
				return SP_MANAGER_NONE;
		}//exit game and go to main window
		gameReset(src->gameWin->game);
		spGameWindowHide(src->gameWin);
		spMainWindowShow(src->mainWin);
		src->activeWin = SP_MAIN_WINDOW_ACTIVE;
	}
	else if (event == SP_GAME_QUIT) {//exit program
		if (!src->gameWin->savedOnLastTurn){//if game is not saved - open message box
			saveBeforeExitButton = createSaveBeforeExitMessageBox();
			if (saveBeforeExitButton==1){//game is saved
				spManagerSaveGameToSlot(src);
			}
			else if (saveBeforeExitButton==2)//cancel - stay in game
				return SP_MANAGER_NONE;
		}
		return SP_MANAGER_QUIT;
	}
	else if (event != SP_GAME_UNDO && event != SP_GAME_RESTART && event != SP_GAME_NONE && event != SP_GAME_INVALID_ARGUMENT){//move event
		statusAfterMove = handleManagerDueToGameMoveEvent(src, event);//get game status after move
		if (statusAfterMove==1 && src->gameWin->game->gameSettings->gameMode == SP_CHESS_GAME_1_PLAYER_MODE){//if 1 player game and game not over
			if (!spManagerHandleComputerMove(src)){//play computer move - if an allocation error occurred exit
				printf("ERROR: malloc has failed");
				return SP_MANAGER_QUIT;
			}
		}
		else if (statusAfterMove==0){//if game over leave game window and go to main window
			gameReset(src->gameWin->game);
			spGameWindowHide(src->gameWin);
			spMainWindowShow(src->mainWin);
			src->activeWin = SP_MAIN_WINDOW_ACTIVE;
		}
		if (statusAfterMove!=-1)
			src->gameWin->savedOnLastTurn = false;//after a valid turn - update that game wasn't saved after last turn
	}
	return SP_MANAGER_NONE;
}

SP_MANAGER_EVENET spManagerHandleEvent(SPGuiManager* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_MANAGER_QUIT;
	}
	if (src->activeWin == SP_MAIN_WINDOW_ACTIVE) {//handle main window event
		SP_MAIN_EVENT mainEvent = spMainWindowHandleEvent(src->mainWin, event);
		return handleManagerDueToMainEvent(src, mainEvent);
	} else if (src->activeWin == SP_GAME_MODE_WINDOW_ACTIVE){//handle game moded window event
		SP_GAME_MODE_EVENT gameModeEvent = spGameModeWindowHandleEvent(src->gameModeWin, event);
		spManagerDraw(src);
		return handleManagerDueToGameModeEvent(src, gameModeEvent);
	} else if (src->activeWin == SP_DIFFICULTY_LEVEL_WINDOW_ACTIVE){//handle difficulty level window event
		SP_DIFFICULTY_LEVEL_EVENT difficultyLevelEvent = spDifficultyLevelWindowHandleEvent(src->difficultyLevelWin, event);
		spManagerDraw(src);
		return handleManagerDueToDifficultyLevelEvent(src, difficultyLevelEvent);
	} else if (src->activeWin == SP_USER_COLOR_WINDOW_ACTIVE){//handle user color window event
		SP_USER_COLOR_EVENT userColorEvent = spUserColorWindowHandleEvent(src->userColorWin, event);
		spManagerDraw(src);
		return handleManagerDueToUserColorEvent(src, userColorEvent);
	} else if (src->activeWin == SP_LOAD_WINDOW_ACTIVE){//handle load window event
		SP_LOAD_EVENT loadEvent = spLoadWindowHandleEvent(src->loadWin, event);
		spManagerDraw(src);
		return handleManagerDueToLoadEvent(src, loadEvent);
	} else if (src->activeWin == SP_GAME_WINDOW_ACTIVE){//handle game window event
		SP_GAME_EVENT gameEvent = spGameWindowHandleEvent(src->gameWin, event);
		spManagerDraw(src);
		return handleManagerDueToGameEvent(src, gameEvent);
	}

	return SP_MANAGER_NONE;
}
