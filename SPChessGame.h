#ifndef SPCHESSGAME_H_
#define SPCHESSGAME_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "SPArrayList.h"
#include "SPChessInit.h"
#include "SPChessMove.h"

/**
 * SPChessGame Summary:
 *
 * A container that represents a classic chess game, a two players 8 by 8
 * board game (rows X columns). The container supports the following functions.
 *
 * spChessGameCreate				- Creates a new game board
 * spChessGameCopy					- Copies a game board
 * pieceCopy					- Copies a player piece
 * kingCopy					- Copies a player king
 * spChessGameDestroy				- Frees all memory resources associated with a game
 * isBlackPiece			- Checks if a Piece is black
 * isWhitePiece           - Checks if a piece is white
 * isValidPosition		- Checks if a given index is out of board range
 * isValidPieceInPosition	- Checks if a given position on board doesn't contain a player's piece
 * isSameColorInDst		- Checks if a player wants to move a piece to a position containing another piece of his
 * isLegalMove			- Checks if a move is legal
 * spChessGamekingWillBeThreatened	- Checks if the player's king will be threatened by his move
 * spChessGamepawnMoveIsValid		- Checks if the move from source position to destination position is a valid pawn move
 * spChessGamepawnCapturingIsValid	- Checks if the move from source position to destination position is a valid pawn capturing
 * spChessGamebishopMoveIsValid		- Checks if the move from source position to destination position is a valid bishop move
 * spChessGamerookMoveIsValid		- Checks if the move from source position to destination position is a valid rook move
 * spChessGamequeenMoveIsValid		- Checks if the move from source position to destination position is a valid queen move
 * spChessGameknightMoveIsValid		- Checks if the move from source position to destination position is a valid knight move
 * spChessGamekingMoveIsValid		- Checks if the move from source position to destination position is a valid king move
 * spChessGameisCheck				- Checks if there's a check on a player's king
 * spChessGamecreateDemoSpChessGame - Copies a given game and sets a move on the copy
 * spChessGameSetMove				- Sets a move on a game board
 * spChessGameIsValidMove			- Checks if a move is valid
 * spChessGameUndoPrevMove			- Undoes previous move made by the last player
 * spChessGamePrintBoard			- Prints the current board
 * spChessGameGetCurrentPlayer		- Returns the current player
 * spChessGameinitializeGameBoard	- Initializes a chess game board
 * spChessGameinitializePiecesArrays - Initializes the game pieces arrays
 * spChessGameSaveToFile			- Saves a given game to a given file
 * spChessGameLoadFromFile			- Loads a game from a given file
 * spChessGameCheckCheckmateOrTie	- Checks game status - check, checkmate, tie, or no end of game
 * spChessGameNoLegalMove			- Checks if there are no legal move for a player on the game board
 *
 */

//Definitions
//chess pieces
#define SP_CHESS_GAME_SPAN 8
#define SP_CHESS_GAME_BLACK_PAWN 'M'
#define SP_CHESS_GAME_BLACK_BISHOP 'B'
#define SP_CHESS_GAME_BLACK_ROOK 'R'
#define SP_CHESS_GAME_BLACK_KNIGHT 'N'
#define SP_CHESS_GAME_BLACK_QUEEN 'Q'
#define SP_CHESS_GAME_BLACK_KING 'K'

#define SP_CHESS_GAME_WHITE_PAWN 'm'
#define SP_CHESS_GAME_WHITE_BISHOP 'b'
#define SP_CHESS_GAME_WHITE_ROOK 'r'
#define SP_CHESS_GAME_WHITE_KNIGHT 'n'
#define SP_CHESS_GAME_WHITE_QUEEN 'q'
#define SP_CHESS_GAME_WHITE_KING 'k'

#define SP_CHESS_GAME_PAWN_STR "pawn"
#define SP_CHESS_GAME_BISHOP_STR "bishop"
#define SP_CHESS_GAME_ROOK_STR "rook"
#define SP_CHESS_GAME_KNIGHT_STR "knight"
#define SP_CHESS_GAME_QUEEN_STR "queen"
#define SP_CHESS_GAME_KING_STR "king"

#define SP_CHESS_GAME_EMPTY_ENTRY '_'

#define SP_CHESS_GAME_A_ASCII_VALUE 65
#define SP_CHESS_GAME_H_ASCII_VALUE 72

//files macros
#define SP_CHESS_FILE_FIRST_LINE "?xml version=\"1.0\" encoding=\"UTF-8\"?"
#define SP_CHESS_FILE_CURRENT_TURN "current_turn"
#define SP_CHESS_FILE_GAME_MODE "game_mode"
#define SP_CHESS_FILE_DIFFICULTY "difficulty"
#define SP_CHESS_FILE_USER_COLOR "user_color"
#define SP_CHESS_FILE_BOARD "board"
#define SP_CHESS_FILE_ROW "row_"
#define SP_CHESS_FILE_GENERAL "general"

//a structure that represents a chess piece.
typedef struct sp_chess_piece_ {
	bool isAlive; //true if the piece is still in the game (hasn't been captured), false o.w.
	char type; //holds the type of the piece
	int row; //holds the row location
	int column; //holds the column location
} SPChessPiece;

//a structure that represents a chess king piece.
typedef struct sp_chess_king_ {
	int kingRow; //holds the king's row location
	int kingCol; //holds the king's column location
	bool isKingCheck; //true if there is a check on the king, false o.w.
} SPChessKing;


typedef struct sp_chess_game_t {
	char gameBoard[SP_CHESS_GAME_SPAN][SP_CHESS_GAME_SPAN]; //represents a chess game board
	int currentPlayer;
	SPArrayList* history;
	SPChessPiece currWhitePieces[SP_CHESS_GAME_SPAN*2]; //holds the white pieces
	SPChessPiece currBlackPieces[SP_CHESS_GAME_SPAN*2]; //holds the black pieces
	SPChessKing* whiteKing; //white king info
	SPChessKing* blackKing; //black king info
	SPChessSettings* gameSettings;
} SPChessGame;

/**
 * Type used for returning error codes from game functions
*/
typedef enum sp_chess_game_message_t {
	SP_CHESS_GAME_INVALID_POSITION, //out of board range
	SP_CHESS_GAME_INVALID_PIECE, //the source position doesn't contain a valid piece
	SP_CHESS_GAME_ILLEGAL_MOVE,
	SP_CHESS_GAME_INVALID_FILE,
	SP_CHESS_GAME_2_PLAYERS_MODE,
	SP_CHESS_GAME_INVALID_ARGUMENT,
	SP_CHESS_GAME_NO_HISTORY,
	SP_CHESS_GAME_SUCCESS,
	SP_CHESS_GAME_ALLOCATION_ERROR
} SP_CHESS_GAME_MESSAGE;


/**
 * Type used for returning game status from game functions
*/
typedef enum sp_chess_game_status_t {
	SP_CHESS_GAME_WHITE_KING_CHECK,
	SP_CHESS_GAME_WHITE_KING_CHECKMATE,
	SP_CHESS_GAME_BLACK_KING_CHECK,
	SP_CHESS_GAME_BLACK_KING_CHECKMATE,
	SP_CHESS_GAME_TIE,
	SP_CHESS_GAME_NOT_OVER,
	SP_CHESS_GAME_STATUS_ALLOCATION_ERROR
} SP_CHESS_GAME_STATUS;


/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number of previous moves to store. If the number
 * of moves played so far exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @param historySize - The total number of moves to undo,
 *                a player can undo at most historySize move turns.
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
SPChessGame* spChessGameCreate(int historySize);

/**
 * Initializes the piece's arrays- currWhitePieces and currBlackPieces- according to
 * the pieces positions on the board.
 * If there was less than 16 pieces for one of the players, then fills the remaining positions
 * in the array with invalid pieces.
 *
 * @param src - the source game for which we initialize the arrays
 *
 */
void initializePiecesArrays(SPChessGame* src);

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, a new copy of the source game is returned.
 *
 */
SPChessGame* spChessGameCopy(SPChessGame* src);

/**
 * Creates a demo game of the next move in a given game by copying the given game
 * and setting the given move.
 * Updates all SPChessGame needed.
 *
 *
 * @param src - The given game
 * @param move - The specified move to set
 *
 * @return
 *  a pointer to the new demo game or NULL if malloc error occurred
 */
SPChessGame* createDemoSpChessGame(SPChessGame* src, SPChessMove* move);

/**
 * Sets the given move in a given game.
 * @param src - The target game
 * @param col - The next move to set
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src is NULL or move is NULL
 * SP_CHESS_GAME_INVALID_POSITION - if the either source or destination position are out of bound.
 * SP_CHESS_GAME_INVALID_PIECE - if the source position doesn't contain a current player's piece.
 * SP_CHESS_GAME_ILLEGAL_MOVE - if either there is a current player's piece in destination or the move is illegal
       	   	   	   	   	   	   	   for the piece or the king will be threatened after the move.
 * SP_CHESS_GAME_ALLOCATION_ERROR - if there was a malloc allocation error.
 *
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSetMove(SPChessGame* src, SPChessMove* move);

/**
 * Checks if the given move can be set in the given game.
 *
 * @param src - The source game
 * @param col - The move to set
 * @return
 * SP_CHESS_GAME_INVALID_POSITION - if the either source or destination position are out of bound.
 * SP_CHESS_GAME_INVALID_PIECE - if the source position doesn't contain a current player's piece.
 * SP_CHESS_GAME_ILLEGAL_MOVE - if either there is a current player's piece in destination or the move is illegal
       	   	   	   	   	   	   	   for the piece or the king will be threatened after the move.
 * SP_CHESS_GAME_ALLOCATION_ERROR - if there was a malloc allocation error.
 * SP_CHESS_GAME_SUCCESS - if the move is valid.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMove(SPChessGame* src, SPChessMove* move);

/**
 * Undo the last move that was made in the previous turn by moving the piece back to the source
 * position and reviving the captured piece if there was one. The function changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row or if the game mode is a two player mode then an error occurs.
 *
 * @param src - The source game
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src is NULL
 * SP_CHESS_GAME_2_PLAYERS_MODE - if the game mode is a two player mode
 * SP_CHESS_GAME_NO_HISTORY - if the user invoked this function more then historySize in a row.
 * SP_CHESS_GAME_SUCCESS - on success. The last move has been undone.
 */
SP_CHESS_GAME_MESSAGE spChessGameUndoPrevMove(SPChessGame* src);

/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing. The white and black pieces are represented by
 * lower and upper cases letters, respectively. the presentation:
 * m/M, r/R, n/N, b/B, q/Q, k/K for pawn, rook, knight, bishop, queen and king, respectively.
 *
 * @param src - the source game
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src is NULL
 * SP_CHESS_GAME_SUCCESS - otherwise
 *
 */
SP_CHESS_GAME_MESSAGE spChessGamePrintBoard(SPChessGame* src);

/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * -1 - if src is NULL
 * SP_CHESS_GAME_BLACK_COLOR - if it's black player's turn
 * SP_CHESS_GAME_WHITE_COLOR - if it's white player's turn
 */
int spChessGameGetCurrentPlayer(SPChessGame* src);

/**
 * Initializes a given gameBoard entries to the start mode of a chess game,
 * where the black pieces are in rows 0,1 and the white pieces are in rows 6,7
 * both in the right order.
 * pre-condition: the given parameter is valid.
 *
 * @param gameBoard - the given game board to initialize
 *
 */
void initializeGameBoard(char gameBoard[][SP_CHESS_GAME_SPAN]);


/**
 * Saves the src game status to a xml file with a given name. Prints to the file the current player, game settings, game board,
 * pieces arrays and kings pieces in that order.
 *
 * @param src - the source game to save
 * @param fileName - the name of the file to save to
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src is NULL
 * SP_CHESS_GAME_INVALID_FILE - if the file cannot be created or modified
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSaveToFile(SPChessGame* src, char* fileName);

/**
 * Load a saved game status from a xml file of a given name. Saves the game settings, game board,
 * current player, pieces arrays and kings pieces to the given src SPChessGame.
 *
 * @param src - the source game to load to
 * @param fileName - the name of the file to load from
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src is NULL
 * SP_CHESS_GAME_INVALID_FILE - if the file does not exist or cannot be opened
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_INIT_MESSAGE spChessGameLoadFromFile(SPChessGame* src, char* fileName);

/**
* Checks the game status in the given game.
* @param src - the source game
* @return
* SP_CHESS_GAME_STATUS_ALLOCATION_ERROR - if there was a malloc allocation error.
* SP_CHESS_GAME_WHITE_KING_CHECK - if there is a check on the white king (by black player)
* SP_CHESS_GAME_WHITE_KING_CHECKMATE - if there is a checkmate on the white king (by black player)
* SP_CHESS_GAME_BLACK_KING_CHECK - if there is a check on the black king (by white player)
* SP_CHESS_GAME_BLACK_KING_CHECKMATE - if there is a checkmate on the black king (by white player)
* SP_CHESS_GAME_TIE - If the game is over and there's a tie
* SP_CHESS_GAME_NOT_OVER - otherwise
*/
SP_CHESS_GAME_STATUS spChessGameCheckCheckmateOrTie(SPChessGame* src);

/**
 * Checks whether the given piece is a black piece, that is an upper case letter.
 * pre-condition: the given piece is a valid black or white piece (the char represents a chess piece)
 *
 * @param piece - the given piece to check
 * @return
 * true - if the given piece is a black piece
 * false - otherwise
 */
bool isBlackPiece(char piece);

/**
 * Checks whether the given piece is a white piece, that is an lower case letter.
 * pre-condition: the given piece is a valid black or white piece (the char represents a chess piece)
 *
 * @param piece - the given piece to check
 * @return
 * true - if the given piece is a white piece
 * false - otherwise
 */
bool isWhitePiece(char piece);

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void spChessGameDestroy(SPChessGame* src);


#endif
