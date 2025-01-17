#include "history.h"

/*** HISOTRY STRUCT FUNCTIONS ***/

void makeEmptyHistory(History *history)
{
	history->totalSize = 0;
	history->shortTermHistoryIndex = 0;
	makeEmptyShortHistoryArray(history->shortTermHistory);
	LongTermHistoryList *lst = (LongTermHistoryList *)ver_malloc(sizeof(LongTermHistoryList));
	makeEmptyHistoryList(lst);
	history->longTermHistory = lst;
}

void addPromptToHistoryDatabase(History *history, char *prompt)
{
	char *copiedPrompt = (char *)ver_malloc(sizeof(char) * (strlen(prompt) + 1));
	strcpy(copiedPrompt, prompt);
	if (history->totalSize >= SHORT_TERM_HISTROY_SIZE) // replace older prompt in short history. add older prompt to long term list
		insertDataToEndHistoryList(history->longTermHistory, history->shortTermHistory[history->shortTermHistoryIndex]);
	history->shortTermHistory[history->shortTermHistoryIndex] = copiedPrompt;
	history->shortTermHistoryIndex = (history->shortTermHistoryIndex + 1) % SHORT_TERM_HISTROY_SIZE;
	history->totalSize = history->totalSize + 1;
}

char *getLastPrompt(History *history)
{
	if (history->totalSize == 0)
		return NULL; // no prompts were given yet
	return history->shortTermHistory[(history->shortTermHistoryIndex + SHORT_TERM_HISTROY_SIZE - 1) % SHORT_TERM_HISTROY_SIZE];
}

char *getPromptNumber(History *history, int promptNumber)
{
	char *prompt;
	if (promptNumber < 1 || promptNumber > history->totalSize) // prompt number doesn't exist in history
		prompt = NULL;
	else if (promptNumber <= history->longTermHistory->size) // prompt is in long term history list
	{
		LongTermHistoryNode *promptNode = getHistoryNodeAtIndex(history->longTermHistory, promptNumber - 1); // index starts at 0, prompt numbers start at 1
		prompt = promptNode->prompt;
	}
	else // prompt is in short term history
		prompt = history->shortTermHistory[(promptNumber + SHORT_TERM_HISTROY_SIZE - 1) %  SHORT_TERM_HISTROY_SIZE]; // index starts at 0, prompt numbers start at 1
	return prompt;
}

void printHistory(History *history)
{
	printHistoryList(history->longTermHistory);
	printShortTermHistory(history);
}

void readHistoryFromTextFile(History *history, char *fname)
{
	FILE *fin;
	int length;
	char *command;
	fin = fopen(fname, "r");
	if (!fin && errno == 2) // errno 2 is "No such file or directory" error - could happen if no previous runs occured or history was deleted
		return;
	verifyFileOpen(fin, fname);

	while (fscanf(fin, "%d ", &length) != EOF)
	{
		command = (char *)ver_malloc(sizeof(char) * (length + 1));
		fgets(command, length + 1, fin);
		addPromptToHistoryDatabase(history, command);
		free(command);
	}
	fclose(fin);
}

void writeHistoryToTextFile(History *history, char *fname)
{	
	if (history->totalSize == 0) // no history - nothing to write
		return;

	FILE *fout;
	fout = fopen(fname, "w");
	verifyFileOpen(fout, fname);

	// write to file from oldest command to newest
	// start with long term history - older commands
	writeLongTermHistoryListToTextFile(fout, history->longTermHistory);
	
	// continue to write short term history - newest commands
	writeShortTermHistoryToTextFile(fout, history);
	fclose(fout);
}

void freeHistory(History *history)
{
	freeShortTermHistroy(history->shortTermHistory);
	freeHistoryList(history->longTermHistory);
}

/*** SHORT TERM HISOTRY ARRAY FUNCTIONS ***/

void makeEmptyShortHistoryArray(char *shortTermHistory[])
{
	int i;
	// initalize all short term history cells to null
	for (i = 0; i < SHORT_TERM_HISTROY_SIZE; i++)
		shortTermHistory[i] = NULL;
}

void printShortTermHistory(History *history)
{
	int i;
	if (history->totalSize < SHORT_TERM_HISTROY_SIZE)
	{
		// still not completed a full "loop" in short term history. Print only until index, which is number of commands in database
		for (i = 0; i < history->shortTermHistoryIndex; i++)
			printf("%d: %s\n", i + 1, history->shortTermHistory[i]);
	}
	else
	{
		// completed more than one "loop" in short term history, also has command in long term history
		int longTermSize = history->longTermHistory->size;
		for (i = 0; i < SHORT_TERM_HISTROY_SIZE; i++)
			printf("%d: %s\n", longTermSize + i + 1, history->shortTermHistory[(history->shortTermHistoryIndex + i) % SHORT_TERM_HISTROY_SIZE]);
	}
}

void writeShortTermHistoryToTextFile(FILE *fp, History *history)
{
	int i;
	char *command;

	// still not completed a full "loop" in short term history, write only until index, which is number of commands in database
	if (history->totalSize < SHORT_TERM_HISTROY_SIZE)
	{
		for (i = 0; i < history->shortTermHistoryIndex; i++)
		{
			command = history->shortTermHistory[i];
			fprintf(fp, "%zd %s\n", strlen(command), command);
		}
	}
	// completed more than one "loop" in short term history, write latest commands in this loop
	else
	{
		for (i = 0; i < SHORT_TERM_HISTROY_SIZE; i++)
		{
			command = history->shortTermHistory[(history->shortTermHistoryIndex + i) % SHORT_TERM_HISTROY_SIZE];
			fprintf(fp, "%zd %s\n", strlen(command), command);
		}
	}
}

void freeShortTermHistroy(char *shortTermHistory[])
{
	int i;
	for (i = 0; i < SHORT_TERM_HISTROY_SIZE; i++)
		free(shortTermHistory[i]); // free each prompt from short history
}

/*** LONG TERM HISOTRY LIST FUNCTIONS ***/

void makeEmptyHistoryList(LongTermHistoryList *lst)
{
	lst->head = lst->tail = NULL;
	lst->size = 0;
}

bool isEmptyHistoryList(LongTermHistoryList *lst)
{
	return lst->size == 0;
}

LongTermHistoryNode *createNewHistoryListNode(char *prompt, LongTermHistoryNode *next)
{
	LongTermHistoryNode *res = (LongTermHistoryNode *)ver_malloc(sizeof(LongTermHistoryNode));
	res->prompt = prompt;
	res->next = next;
	return res;
}

void insertDataToEndHistoryList(LongTermHistoryList *lst, char *prompt)
{
	LongTermHistoryNode *newTail;
	newTail = createNewHistoryListNode(prompt, NULL);
	insertNodeToEndHistoryList(lst, newTail);
}

void insertNodeToEndHistoryList(LongTermHistoryList *lst, LongTermHistoryNode *tail)
{
	if (isEmptyHistoryList(lst))
		lst->head = lst->tail = tail;
	else
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
	lst->size = lst->size + 1;
}

LongTermHistoryNode *getHistoryNodeAtIndex(LongTermHistoryList *lst, int index)
{
	if (index < 0 || index > lst->size) // requested index is out of bounds
		return NULL;
	LongTermHistoryNode *curr = lst->head;
	while (curr != NULL && index > 0)
	{
		curr = curr->next;
		index--;
	}
	return curr;
}

void printHistoryList(LongTermHistoryList *lst)
{
	int i;
	LongTermHistoryNode *curr = lst->head;
	for (i = 0; i < lst->size; i++)
	{
		printf("%d: %s\n", i + 1, curr->prompt);
		curr = curr->next;
	}
}

void writeLongTermHistoryListToTextFile(FILE *fp, LongTermHistoryList *lst)
{
	LongTermHistoryNode *curr = lst->head;
	while (curr != NULL)
	{
		fprintf(fp, "%zd %s\n", strlen(curr->prompt), curr->prompt);
		curr = curr->next;
	}
}

void freeHistoryList(LongTermHistoryList *lst)
{
	LongTermHistoryNode *curr = lst->head, *tmp;
	while (curr != NULL)
	{
		tmp = curr->next;
		freeHistoryNode(curr);
		curr = tmp;
	}
	free(lst); // long term list itself is also allocated memory
}

void freeHistoryNode(LongTermHistoryNode *node)
{
	free(node->prompt);
	free(node);
}
