#ifndef __HISTORY_H
#define __HISTORY_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "memoryUtils.h"
#include "fileUtils.h"

#define SHORT_TERM_HISTROY_SIZE 7

typedef struct longTermHistoryNode
{
	char *prompt; // data is prompt (input) string
	struct longTermHistoryNode *next;
} LongTermHistoryNode;

typedef struct longTermHistoryList
{
	int size;
	LongTermHistoryNode *head;
	LongTermHistoryNode *tail;
} LongTermHistoryList;

typedef struct history
{
	int shortTermHistoryIndex;
	char *shortTermHistory[SHORT_TERM_HISTROY_SIZE];
	LongTermHistoryList *longTermHistory;
	int totalSize;
} History;


/*** HISTORY STRUCT FUNCTIONS ***/

/*
* Creates an empty history database, given a database address
*/
void makeEmptyHistory(History *history);

/*
* Given a history database and prompt, add the prompt to the history db
*/
void addPromptToHistoryDatabase(History *history, char *prompt);

/*
* Given a history database, get the last prompt entered into it.
* Returns NULL if not found
*/
char *getLastPrompt(History *history);

/*
* Given a prompt number, find and return this prompt in history database.
* Returns NULL if not found
*/
char *getPromptNumber(History *history, int promptNumber);

/*
* Given a history database, prints all the prompts in the history.
* Prints prompts with their index in the database
*/
void printHistory(History *history);

/*
* Read a history saved in a text file and add it to the history struct given
*/
void readHistoryFromTextFile(History *history, char *fname);

/*
* Write a given history struct to a text file with the givn file name
*/
void writeHistoryToTextFile(History *history, char *fname);

/*
* Frees a history database allocated memory
*/
void freeHistory(History *history);


/*** SHORT TERM HISTORY ARRAY FUNCTIONS ***/

/*
* Creates an empty short term history array, given an array reference.
* Initializes all cells to NULL
*/
void makeEmptyShortHistoryArray(char *shortTermHistory[]);

/*
* Given a history database, prints all the prompts in the short term history database.
* Prints prompts with their index in the database
*/
void printShortTermHistory(History *history);

/*
* Given a history database, writes all the prompts in the short term history database
* to the given open (!) text file (!) fp. Prefixes each prompt in the file with its length
*/
void writeShortTermHistoryToTextFile(FILE *fp, History *history);

/*
* Frees the allocated memory of a short term history array
*/
void freeShortTermHistroy(char *shortTermHistory[]);


/*** LONG TERM HISTORY LIST FUNCTIONS ***/

/*
* Creates an empty long term history list, given a list address
*/
void makeEmptyHistoryList(LongTermHistoryList *lst);

/*
* Check if a history list is empty (length = 0). Returns true if empty, false otherwise
*/
bool isEmptyHistoryList(LongTermHistoryList *lst);

/*
* Creates a new long term history list node that can be added to a list.
* Requires a prompt string, and reference to the next nodes
*/
LongTermHistoryNode *createNewHistoryListNode(char *prompt, LongTermHistoryNode *next);

/*
* Given a history list and prompt, inserts a new node with this command data to the end of the list.
*/
void insertDataToEndHistoryList(LongTermHistoryList *lst, char *prompt);

/*
* Given a history list and a new tail node, inserts the new tail into the list, updating refrences as needed
*/
void insertNodeToEndHistoryList(LongTermHistoryList *lst, LongTermHistoryNode *tail);

/*
* Returns the history node at index. If not found, returns NULL
*/
LongTermHistoryNode *getHistoryNodeAtIndex(LongTermHistoryList *lst, int index);

/*
* Prints all the commands kept in the given history list. Prints with index prefix for each command
*/
void printHistoryList(LongTermHistoryList *lst);

/*
* Given a long term history list, writes all the prompts in the list
* to the given open (!) text file (!) fp. Prefixes each prompt in the file with its length
*/
void writeLongTermHistoryListToTextFile(FILE *fp, LongTermHistoryList *lst);

/*
* Frees the allocated memory of a history list (frees each node's memory)
*/
void freeHistoryList(LongTermHistoryList *lst);

/*
* Frees the allocated memory of a history node
*/
void freeHistoryNode(LongTermHistoryNode *node);


#endif // !__HISTORY_H
