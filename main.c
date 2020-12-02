/*
Student names: Liron Kotev, Idan Cohen
IDs: 318598380, 204363451

Course's final project - console application to manage real estate apartments
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define SHORT_TERM_HISTROY_SIZE 7
#define BUFFER_SIZE 50

/* commands */

#define FIND_APT "find-apt"
#define ADD_APT "add-apt"
#define BUY_APT "buy-apt"
#define DELETE_APT "delete-apt"
#define EXIT "exit"
#define LAST_COMMAND "!!"
#define SHORT_HISTORY "short_history"
#define HISTORY "history"
// run command number: !<num>
// run command number and replace str1 with str2: !<num>^str1^str2

/* data structs */

char *shortTermHistory[SHORT_TERM_HISTROY_SIZE];

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
	// TODO: decide if we want double-linked list
	// struct apartmentNode *prev;
} ApartmentNode;

typedef struct apartmentList
{
	ApartmentNode *head;
	ApartmentNode *tail;
} ApartmentList;

typedef struct commandNode
{
	char *command; // data is commands string
	struct commandNode *next;
} CommandNode;

typedef struct longTermHistoryList
{
	CommandNode *head;
	CommandNode *tail;
} LongTermHistoryList;

void main()
{
	// start of program instructions prints
	puts("Please enter one of the following commands :");
	puts("add - apt, find - apt, buy - apt, delete - apt or exit");
	puts("For reconstruction commands, please enter :");
	puts("!!, !num, history, short_history or !num^str1^str2");

	char *input;
	input = getInput(); // get first prompt
	while (!(strcmp(input, EXIT))) // run until exit
	{
		/*
		separate to command and arguments
		run wanted command according to arguments
		save prompt if command needs to be saved
		*/
		command, arguments = splitPromptToCommandAndArguments(input);
		if (strcmp(command, HISTORY))
			printHistory();
		else if (strcmp(command, SHORT_HISTORY))
			printShortHistory();
		else if (strcmp(command, LAST_COMMAND))
			printLastCommand();
		else if (command == "!<num>")
			printCommandNumber();
		else if (command == "!<num>^str1^str2")
			printCommandNumberWithReplace();
		else  // command is one of: find-apt, add-apt, buy-apt, delete-apt
		{
			if (strcmp(command, FIND_APT))
				findApt(arguments);
			else if (strcmp(command, ADD_APT))
				addApt(arguments);
			else if (strcmp(command, BUY_APT))
				buyApt(arguments);
			else if (strcmp(command, DELETE_APT))
				deleteApt(arguments);
			addPromptToDatabase(input);
		}
		input = getInput();  // get next prompt
	}

	// end of program
	writeToPromtsTextFile();
	writeToApartmentsBinaryFile();
	freeMemory();
	puts("Good Bye!");
}

/*
* Reads a line of unknown length from stdin, until a new-line is given (\n char).
* Returns the line as a string
*/
char * getInput()
{
	unsigned int allocSize = BUFFER_SIZE, length = 0;
	char *input;
	char ch;
	input = (char *)ver_malloc(allocSize * sizeof(char));
	// get input in chunks until the end ('\n' is given)
	scanf("%c", &ch); // get first character
	while (ch != '\n')
	{
		input[length++] = ch;
		if (length == allocSize)
		{
			allocSize += BUFFER_SIZE; // increase allocation size
			input = (char *)ver_realloc(input, allocSize * sizeof(char));
		}
		scanf("%c", &ch);
	}
	input[length++] = '\0';  // add finalizer string character
	input = (char *)ver_realloc(input, length * sizeof(char)); // re-allocate according to exact string length
	return input;
}

/*
* Allocates memory using malloc, verifying allocation succeeded.
* If allocation fails, prints an error message and exits the program
*/
void* ver_malloc(size_t size)
{
	void* res = malloc(size);
	verifyAllocation(res);
	return res;
}

/*
* Re-allocates memory using realloc, verifying allocation succeeded.
* If allocation fails, prints an error message and exits the program
*/
void* ver_realloc(void *data, size_t size)
{
	void* res = realloc(data, size);
	verifyAllocation(res);
	return res;
}

/*
* verifies an array is allocated (not null).
* throws error message and exits if array isn't allocated.
* should be used after allocating new memory to a variable
*/
void verifyAllocation(void *arr)
{
	if (!arr)
	{
		printf("Memory allocation failed - exiting!");
		exit(1);
	}
}

