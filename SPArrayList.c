/*
 * SPArrayList.c
 *
 *  Created on: 10 ���� 2017
 *      Author: michal
 */

#include "SPArrayList.h"


SPArrayList* spArrayListCreate(int maxSize){
	if (maxSize <= 0 ) //invalid maxSize
		return NULL;

	SPArrayList* history = (SPArrayList*)malloc(sizeof(SPArrayList));//initializes arrayList
	if (history == NULL)//malloc error
		return NULL;

	history->elements = (SPArrayListElement**)malloc(maxSize*sizeof(SPArrayListElement*));//initializes arrayList's elements
	if (history->elements==NULL){//malloc error
		free(history);
		return NULL;
	}

	history->actualSize = 0;//initializes arrayLists actualSize to zero
	history->maxSize = maxSize;//initializes arrayLists maxSize according to given argument
	return history;
}

SPArrayListElement* spArrayListCreateElement(){
	SPArrayListElement* elem = (SPArrayListElement*)malloc(sizeof(SPArrayListElement));
	if (elem==NULL) //malloc error
		return NULL;

	//initializes fields
	elem->move = createMove(0,0,0,0); //creates move
	if (elem->move==NULL){ //malloc error
		listElementDestroy(elem);
		return NULL;
	}

	elem->isCheck = false;
	elem->pieceInDst = '_';

	return elem;
}

SPArrayListElement* spArrayListCopyElement(SPArrayListElement* src){
	if (src == NULL)
		return NULL;
	SPArrayListElement* copy = spArrayListCreateElement(); //creates copy
	if (copy == NULL)//malloc error
		return NULL;

	//initializes copy fields to be the same as the given arrayList's element
	setMove(copy->move, src->move->srcRow, src->move->srcCol, src->move->dstRow, src->move->dstCol);
	copy->isCheck = src->isCheck;
	copy->pieceInDst = src->pieceInDst;

	return copy;
}

void spArrayListSetElement(SPArrayListElement* elem, int srcRow, int srcCol, int dstRow, int dstCol, char pieceInDst, int isCheck){
	setMove(elem->move, srcRow, srcCol, dstRow, dstCol);
	elem->pieceInDst = pieceInDst;
	elem->isCheck = isCheck;
}

SPArrayList* spArrayListCopy(SPArrayList* src){
	if (src == NULL)
		return NULL;
	SPArrayListElement* elem;
	SPArrayList* copy = spArrayListCreate(src->maxSize);//creates copy arrayList with the given src maxSize
	if (copy == NULL) //malloc error
		return NULL;

	for (int i=0;i<src->actualSize;i++){//copies given arrayList to "copy"
		elem = spArrayListCopyElement(spArrayListGetAt(src, i));
		if (elem == NULL){ //malloc error
			spArrayListDestroy(copy);
			return NULL;
		}
		spArrayListAddLast(copy, elem);
	}

	copy->actualSize = src->actualSize;

	return copy;
}


void spArrayListDestroy(SPArrayList* src){
	if (src==NULL)
		return;
	if (src->elements!=NULL){
		for (int i=0;i<src->actualSize;i++)//frees all elements in elements
			listElementDestroy(src->elements[i]);
		free(src->elements);
	}
	free(src);
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if (src==NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if (src->elements!=NULL){ //clears list's elements
		for (int i=0;i<src->actualSize;i++)//frees all elements in elements
			listElementDestroy(src->elements[i]);
	}

	src->actualSize = 0; //changing actualSize to 0
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, SPArrayListElement* elem, int index){
	if (src==NULL || elem==NULL || index<0 || index > src->actualSize)//if index is out of bound or src==NULL returns an "invalid" message
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (spArrayListIsFull(src))//if arrayList is full returns "full" message
			return SP_ARRAY_LIST_FULL;
	if (index < src->actualSize)//if index is less then actualSize
		for (int i=src->actualSize-1; i>=index; i--)//shifts all other elements accordingly
			src->elements[i+1] = src->elements[i];
	src->elements[index] = elem;//adds new element in given index
	src->actualSize++;//increase actual size by 1
	return SP_ARRAY_LIST_SUCCESS;//returns "success" message
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, SPArrayListElement* elem){
	return spArrayListAddAt(src, elem, 0);//adds to first position by spArrayListAddAt on index 0
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, SPArrayListElement* elem){
	if (src == NULL || elem == NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	return spArrayListAddAt(src, elem, src->actualSize);//adds to last position by spArrayListAddAt on index actualSize
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if (src==NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if (spArrayListIsEmpty(src))//if arrayList is empty returns "empty" message
		return SP_ARRAY_LIST_EMPTY;

	if (index<0 || index >= src->actualSize)//if index is out of bound returns an "invalid" message
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	listElementDestroy(src->elements[index]); //"removes" element in index by destroying the element
	if (index < src->actualSize-1){ //index is less then actualSize
		for (int i=index; i<src->actualSize-1; i++) //shifts all other elements accordingly
			src->elements[i] = src->elements[i+1];
	}
	src->elements[src->actualSize-1] = NULL; //resets the last index
	src->actualSize--;//reduces actualSize by 1
	return SP_ARRAY_LIST_SUCCESS;// returns "success" message
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);//removes from first position by spArrayListRemoveAt on index 0
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if (src == NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	return spArrayListRemoveAt(src, src->actualSize-1);//removes from last position by spArrayListRemoveAt on index actualSize-1
}

SPArrayListElement* spArrayListGetAt(SPArrayList* src, int index){
	if (src==NULL || index<0 || index >= src->actualSize)//if index is out of bound or src==NULL returns -1
		return NULL;
	return src->elements[index];//returns the element in the given index
}

SPArrayListElement* spArrayListGetFirst(SPArrayList* src){
	return spArrayListGetAt(src, 0);//gets the first element by spArrayListGetAt on index 0
}

SPArrayListElement* spArrayListGetLast(SPArrayList* src){
	if (src == NULL) {
		return NULL;
	}
	return spArrayListGetAt(src, src->actualSize-1);//gets the last element by spArrayListGetAt on index actualSize-1
}


int spArrayListMaxCapacity(SPArrayList* src){
	if(src == NULL)
		return -1;
	return src->maxSize; //returns maxSize
}

int spArrayListSize(SPArrayList* src){
	if(src == NULL)
		return -1;
	return src->actualSize;//returns actualSize
}

bool spArrayListIsFull(SPArrayList* src){
	if (src == NULL) {
		return false;
	}
	return (src->actualSize==src->maxSize);//returns true if full, else false
}

bool spArrayListIsEmpty(SPArrayList* src){
	if (src == NULL) {
		return false;
	}
	return (src->actualSize==0);//returns true if empty, else false
}

void listElementDestroy(SPArrayListElement* src){
	if (src!=NULL){
		destroyMove(src->move);
		free(src);
	}
}
