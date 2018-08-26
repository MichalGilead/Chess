#ifndef SPARRAYLIST_H_
#define SPARRAYLIST_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPChessMove.h"

/**
 * SPArrayList summary:
 *
 * A container that represents a fixed size linked list. Each list's element contains the move that has been done
 * (source and destination), the captured piece if exists, and check state.
 * The capacity of the list is specified at the creation. The container supports typical list
 * functionalities with the addition of random access as in arrays.
 * Upon insertion, if the maximum capacity is reached then an error message is
 * returned and the list is not affected. A summary of the supported functions
 * is given below:
 *
 * spArrayListCreate       - Creates an empty array list with a specified
 *                           max capacity.
 * spArrayListCopy         - Creates an exact copy of a specified array list.
 * spArrayListDestroy      - Frees all memory resources associated with an array
 *                           list.
 * spArrayListClear        - Clears all elements from a specified array list.
 * spArrayListAddAt        - Inserts an element at a specified index, elements
 *                           will be shifted to make place.
 * spArrayListAddFirst     - Inserts an element at the beginning of the array
 *                           list, elements will be shifted to make place.
 * spArrayListAddLast      - Inserts an element at the end of the array list.
 * spArrayListRemoveAt     - Removes an element at the specified index, elements
 *                           elements will be shifted as a result.
 * spArrayListRemoveFirst  - Removes an element from the beginning of the array
 *                           list, elements will be shifted as a result.
 * spArrayListRemoveLast   - Removes an element from the end of the array list
 * spArrayListGetAt        - Accesses the element at the specified index.
 * spArrayListGetFirst     - Accesses the first element of the array list.
 * spArrayListGetLast      - Accesses the last element of the array list.
 * spArrayListMaxCapcity   - Returns the maximum capcity of the array list.
 * spArrayListSize         - Returns the number of elements in that array list.
 * spArrayListIsFull       - Returns if the array list reached its max capacity.
 * spArrayListIsEmpty      - Returns true if the array list contains no elements.
 */

//a structure that represents an array list element.
typedef struct sp_array_element_t {
	SPChessMove* move; //the move that has been done
	char pieceInDst; //the captured piece or empty entry if there was none
	int isCheck; //check state before move- holds -1 for no check, 0 for black king check and 1 for white king check
} SPArrayListElement;

typedef struct sp_array_list_t {
	SPArrayListElement** elements; //an array of history's elements
	int actualSize;
	int maxSize;
} SPArrayList;


/**
 * A type used for errors
 */
typedef enum sp_array_list_message_t {
	SP_ARRAY_LIST_SUCCESS,
	SP_ARRAY_LIST_INVALID_ARGUMENT,
	SP_ARRAY_LIST_FULL,
	SP_ARRAY_LIST_EMPTY
} SP_ARRAY_LIST_MESSAGE;

/**
 * Creates an empty array list with the specified maximum capacity.
 *
 * @param maxSize - the maximum capacity of the target array list.
 * @return
 * NULL, if an allocation error occurred or maxSize  <= 0.
 * An instant of an array list otherwise.
 */
SPArrayList* spArrayListCreate(int maxSize);

/**
 * Creates an array list element.
 *
 * @return
 * NULL, if an allocation error occurred.
 * An instant of an array list element otherwise.
 */
SPArrayListElement* spArrayListCreateElement();

/**
 *	Creates an exact copy of the src array list element.
 *	@param src - the source element.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source element, otherwise.
 */
SPArrayListElement* spArrayListCopyElement(SPArrayListElement* src);

/**
 * Sets the element of a list.
 * pre-condition : The function is called with the assertion that all arguments are valid.
 *
 * @param elem - the element to set
 * @param srcRow - the source row of the move
 * @param srcCol - the source column of the move
 * @param dstRow - the destination row of the move
 * @param dstCol - the destination column of the move
 * @param pieceInDst - the opponent piece that was captured in move or an empty entry
 * @param check - the check state before move
 */
void spArrayListSetElement(SPArrayListElement* elem, int srcRow, int srcCol, int dstRow, int dstCol, char pieceInDst, int isCheck);


/**
 * Creates an exact copy of the src array list. Elements in the new copy will
 * be in the same order as they appeared in the source list.
 *
 * @param src - the source array list.
 * @return
 * NULL if either an allocation error occurs or src == NULL.
 * A new copy of the source array list, otherwise.
 */
SPArrayList* spArrayListCopy(SPArrayList* src);

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 *
 * @param src - the source array list
 */
void spArrayListDestroy(SPArrayList* src);

/**
 * Clears all elements from the source array list. After invoking this function,
 * the size of the source list will be reduced to zero and maximum capacity is
 * not affected.
 *
 * @param src - the source array list
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT if src == NULL
 * SP_ARRAY_LIST_SUCCESS otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src);

/**
 * Inserts element at a specified index. The elements residing at and after the
 * specified index will be shifted to make place for the new element. If the
 * array list reached its maximum capacity an error message is returned and
 * the source list is not affected
 *
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @param index - the index where the new element will be placed. The index is
 *                0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, SPArrayListElement* elem, int index);

/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * array list reached its maximum capacity an error message is returned and
 * the source list is not affected
 *
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL, elem == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, SPArrayListElement* elem);

/**
 * Inserts element at a the end of the source element. If the array list
 * reached its maximum capacity an error message is returned and the source
 * list is not affected.
 *
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
 SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, SPArrayListElement* elem);
/**
 * Removes an element from a specified index. The elements residing after the
 * specified index will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 *
 * @param src   - The source array list
 * @param index - The index from where the element will be removed.
 *                The index is 0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index);

/**
 * Removes an element from a the beginning of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 *
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src);

/**
 * Removes an element from a the end of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 *
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise.
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src);

/**
 * Returns the element at the specified index.
 *
 * @param src - the source array list
 * @param index - the specified index, the index is 0-based.
 * @return
 * NULL if either src == NULL or index out of bound.
 * Otherwise, the element at the specified index is returned.
 */
SPArrayListElement* spArrayListGetAt(SPArrayList* src, int index);

/**
 * Returns the element at the beginning of the list.
 *
 * @param src - the source array list
 * @return
 * NULL if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
SPArrayListElement* spArrayListGetFirst(SPArrayList* src);

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 *
 * @param src - the source array list
 * @return
 * NULL if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
SPArrayListElement* spArrayListGetLast(SPArrayList* src);

/**
 * Returns the maximum capacity of the list.
 *
 * @param src - the source array list
 * @return
 * -1 if src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int spArrayListMaxCapacity(SPArrayList* src);

/**
 * Returns the number of elements in the list.
 *
 * @param src - the source array list
 * @return
 * -1 if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int spArrayListSize(SPArrayList* src);

/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 *
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool spArrayListIsFull(SPArrayList* src);

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 *
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool spArrayListIsEmpty(SPArrayList* src);

/**
 * frees the memory of an array list element. If the
 * source element is NULL, then the function does nothing.
 *
 * @src - the element we want to free
 *
 */
void listElementDestroy(SPArrayListElement* src);

#endif
