#ifndef __HISTORY_H
#define __HISTORY_H

#define SHORT_TERM_HISTROY_SIZE 7

typedef struct longTermHistoryNode
{
	char *command; // data is commands string
	struct longTermHistoryNode *next;
} LongTermHistoryNode;

typedef struct longTermHistoryList
{
	LongTermHistoryNode *head;
	LongTermHistoryNode *tail;
} LongTermHistoryList;


#endif // !__HISTORY_H
