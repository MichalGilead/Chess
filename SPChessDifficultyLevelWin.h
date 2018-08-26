/*
 * SPChessDifficultyLevelWin.h
 *
 *  Created on: 25 ����� 2017
 *      Author: michal
 */

#ifndef GRAPHICS_SPCHESSDIFFICULTYLEVELWIN_H_
#define GRAPHICS_SPCHESSDIFFICULTYLEVELWIN_H_

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//indicator for current active difficulty level
typedef enum{
	SP_NOOB_LEVEL_ACTIVE,
	SP_EASY_LEVEL_ACTIVE,
	SP_MODERATE_LEVEL_ACTIVE,
	SP_HARD_LEVEL_ACTIVE,
}ACTIVE_LEVEL;

//difficulty level window events
typedef enum {
	SP_DIFFICULTY_LEVEL_NOOB,
	SP_DIFFICULTY_LEVEL_EASY,
	SP_DIFFICULTY_LEVEL_MODERATE,
	SP_DIFFICULTY_LEVEL_HARD,
	SP_DIFFICULTY_LEVEL_BACK,
	SP_DIFFICULTY_LEVEL_NEXT,
	SP_DIFFICULTY_LEVEL_INVALID_ARGUMENT,
	SP_DIFFICULTY_LEVEL_NONE,
	SP_DIFFICULTY_LEVEL_QUIT,
} SP_DIFFICULTY_LEVEL_EVENT;

typedef struct {
	SDL_Window* difficultyLevelWindow;
	SDL_Renderer* difficultyLevelRenderer;
	SDL_Texture* activeNoobTexture;
	SDL_Texture* inactiveNoobTexture;
	SDL_Texture* activeEasyTexture;
	SDL_Texture* inactiveEasyTexture;
	SDL_Texture* activeModerateTexture;
	SDL_Texture* inactiveModerateTexture;
	SDL_Texture* activeHardTexture;
	SDL_Texture* inactiveHardTexture;
	SDL_Texture* backTexture;
	SDL_Texture* nextTexture;
	SDL_Texture* bgTexture;
	ACTIVE_LEVEL activeLevel; //holds the active difficulty level button
} SPDifficultyLevelWin;

/*
 * Creates a difficulty level window and all of its fields (textures ,Renderer..) . The window isn't shown at
 * the end of the creation.
 * The function initializes activeLevel to default- SP_EASY_LEVEL_ACTIVE.
 *
 * @return
 * NULL, if an allocation error occurred,
 * An instant of a SPDifficultyLevelWin otherwise.
 */
SPDifficultyLevelWin* spDifficultyLevelWindowCreate();

/*
 * Frees all memory resources associated with the source difficulty level window.
 * If src is NULL the function does nothing.
 *
 * @param src - a pointer to the difficulty level window to destroy
 *
 */
void spDifficultyLevelWindowDestroy(SPDifficultyLevelWin* src);

/*
 * Draws the difficulty level window. Draws the difficulty level's textures as active/inactive according to activeLevel,
 * If src is NULL the function does nothing.
 *
 * @param src - a pointer to the difficulty level window to draw
 */
void spDifficultyLevelWindowDraw(SPDifficultyLevelWin* src);

/*
 * Handle a given event in the difficulty level window.
 *
 * @param src - a pointer to the difficulty level window
 * @param event - a pointer to the event to handle
 * @return
 * SP_DIFFICULTY_LEVEL_INVALID_ARGUMENT - if src is NULL or event is NULL
 * SP_DIFFICULTY_LEVEL_NOOB - if the user clicked on noob level button when it was inactive
 * SP_DIFFICULTY_LEVEL_EASY - if the user clicked on easy level button when it was inactive
 * SP_DIFFICULTY_LEVEL_MODERATE - if the user clicked on moderate level button when it was inactive
 * SP_DIFFICULTY_LEVEL_HARD - if the user clicked on hard level button when it was inactive
 * SP_DIFFICULTY_LEVEL_BACK - if the user clicked on back button
 * SP_DIFFICULTY_LEVEL_NEXT - if the user clicked on next button
 * SP_DIFFICULTY_LEVEL_QUIT - if the user clicked on quit in the window
 * SP_DIFFICULTY_LEVEL_NONE - otherwise
 */
SP_DIFFICULTY_LEVEL_EVENT spDifficultyLevelWindowHandleEvent(SPDifficultyLevelWin* src, SDL_Event* event);

/*
 * Hides the src difficulty level window.
 *
 * @param src - a pointer to the difficulty level window to hide
 */
void spDifficultyLevelWindowHide(SPDifficultyLevelWin* src);

/*
 * Shows the src difficulty level window.
 *
 * @param src - a pointer to the difficulty level window to show
 */
void spDifficultyLevelWindowShow(SPDifficultyLevelWin* src);



#endif /* GRAPHICS_SPCHESSDIFFICULTYLEVELWIN_H_ */
