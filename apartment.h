#ifndef __APARTMENT_H
#define __APARTMENT_H
#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUtils.h"




#define DB_ENTRY_MAX_STRING_LENGTH 11 // xx.xx.xxxx\0

typedef struct apartment
{
	unsigned int id;
	char *address;
	int price;
	short int numRooms;
	short int entryDay;
	short int entryMonth;
	short int entryYear;
	time_t dbEntryDate;
} Apartment;

typedef struct apartmentNode
{
	// data
	Apartment *apt;
	// references
	struct apartmentNode *next;
	struct apartmentNode *prev;
} ApartmentNode;

typedef struct apartmentList
{
	ApartmentNode *head;
	ApartmentNode *tail;
} ApartmentList;

/***** APARTMENT FUNCTIONS ******/

void copyString2(char *string, char *newString);
/*
*Copy the apartment list to the new apartment list
*/
void copyAptList(ApartmentList *lst, ApartmentList *newLst);

/*
*Remove from the apartment list all the apartment from the last days(according to dateNum)
*/
void deleteApt(char *daysNum, ApartmentList *lst);

/*
*Buy an apartment and remove it from the appartment list
*/
void buyApt(char *codeApt, ApartmentList *lst);

/*
*Remove the first signal from the string
*/

char *removeFirstSignal(char *arguments);

/*
*Print the Apartment list with all the apartment data
*/
void printApartmentList(ApartmentList *lst);

/*
*Print the Apartment list sorted by the highest price to the lowest price
*/
void printListByTheHighestPrice(ApartmentList *lst);

/*
*Print the Apartment list sorted by the lowest price to the highest price
*/
void printListByThelowPrice(ApartmentList *lst);

/*
* Creates a new apartment.
*/
Apartment *createNewApartment(unsigned int id, char *address, int price, short int numRoom, short int entryDay, short int entryMonth, short int entryYear);

/*
* Prints an the apartment code to the screen according to given instructions
*/
void printListByApartmentCode(ApartmentList *lst);

/*
* Prints an apartment to the screen according to given instructions
*/
void printApartment(Apartment *apt);

/*
* Frees the allocated memory of an apartment
*/
void freeApartment(Apartment *apt);

/*
* Add a new apartment to the appartment list
*/
void addApt(char *arguments, ApartmentList *lst);


/***** APARTMENTLIST FUNCTIONS ******/

/*
* Creates an empty apartment list, given a list address
*/
void makeEmptyApartmentList(ApartmentList *lst);

/*
* Check if an apartment list is empty (length = 0). Returns true if empty, false otherwise
*/
bool isEmptyApartmentList(ApartmentList *lst);

/*
* Creates a new apartment list node that can be added to a list.
* Requires an apartment reference, and references to the next and previous apartment nodes
*/
ApartmentNode *createNewApartmentListNode(Apartment *apt, ApartmentNode *next, ApartmentNode *prev);

/*
* Given an apartment list and apartment reference, inserts a new node with this apartment.
* Insert to the list is sorted. Updates references as needed
*/
void insertApartmentToList(ApartmentList *lst, Apartment *apt);

/*
* Insert given apartment node to the list, placing it after the requested "after".
* To insert to start of list pass after=NULL
*/
void insertApartmentNodeToList(ApartmentList *lst, ApartmentNode *node, ApartmentNode *after);

/*
* Given an apartment list and a new tail apartment node, inserts the new tail into the list, updating refrences as needed
*/
void insertApartmentNodeToEndList(ApartmentList *lst, ApartmentNode *tail);

/*
* Given an apartment list and a new head apartment node, inserts the new head into the list, updating refrences as needed
*/
void insertApartmentNodeToStartList(ApartmentList *lst, ApartmentNode *head);

/*
* Given an apartment list and price, returns the apartment whose next will be an apartment with given price.
* Meaning, returns node which node->next = [apt node with given price].
* Returns NULL if price is minimal - meaning should be at start of list.
* Assumes x in not in the list - otherwise it shouldn't be used with this method
*/
ApartmentNode *getApartmentNodeBefore(ApartmentList *lst, int price);

/*
* Given an apartment list and an apartment id, removes the apartment with this id from the list. Updates references as needed
*/
void removeApartmentFromListById(ApartmentList *lst, unsigned int id);

/*
* Given an apartment list and amount of days, removes from the list all apartments entered during this time. Updates references as needed
*/
void removeApartmentsFromListByEntryDate(ApartmentList *lst, int numDays);

/*
* Removes a given apartment node from the list. Also frees memory of stored apartment.
* If this is the only node, changes the list to empty list.
* Assumes given node is in the list.
*/
void removeApartmentNodeFromList(ApartmentList *lst, ApartmentNode *node);

/*
* Frees the allocated memory of an apartment list (frees each node's memory)
*/
void freeApartmentList(ApartmentList *lst);

/*
* Frees the allocated memory of an apartment node
*/
void freeApartmentNode(ApartmentNode *node);

#endif // !__APARTMENT_H
