/*
Student names: Liron Kotev, Idan Cohen
IDs: 318598380, 204363451

Course's final project - console application to manage real estate apartments
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUtils.h"
#include "apartment.h"
#include "history.h"

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

void splitPromptToCommandAndArguments(char* prompt, char** pCommand, char** pArguments);
void splitCommandAndArgumentsByToken(char* command, char** pArguments, char token);
char* getInput();


void main()
{
	char *shortTermHistory[SHORT_TERM_HISTROY_SIZE];
	ApartmentList aptsList;
	makeEmptyApartmentList(&aptsList);
	//read appartemnt from fille ,read history from file
	readApartmentsFromBinaryFile(&aptsList);
	readCommandHistoryFromFile();

	// start of program instructions prints
	puts("Please enter one of the following commands :");
	puts("add - apt, find - apt, buy - apt, delete - apt or exit");
	puts("For reconstruction commands, please enter :");
	puts("!!, !num, history, short_history or !num^str1^str2");

	char *input;
	input = getInput(); // get first prompt
	while (strcmp(input, EXIT) != 0) // run until exit
	{
		/*
		separate to command and arguments
		run wanted command according to arguments
		save prompt if command needs to be saved
		*/
		char *command, *arguments;
		splitPromptToCommandAndArguments(input, &command, &arguments);
		if (strcmp(command, HISTORY))
			printHistory();
		else if (strcmp(command, SHORT_HISTORY))
			printShortHistory();
		else if (strcmp(command, LAST_COMMAND))
			printLastCommand();
		else if (command[0] == '!')
			printCommandNumber(command, arguments);
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
		free(command); // this also frees arguments memory
		input = getInput();  // get next prompt
	}
	// end of program
	writeToPromtsTextFile();
	writeToApartmentsBinaryFile();
	freeMemory(); // should also free input string
	puts("Good Bye!");
}

/*
* Splits a given prompt to it's command and arguments. 
* Puts the result in output paramteres pCommand and pArguments.
* If the prompt is only a command, pArguments is set to NULL.
* The function allocates memory for the command and arguments, and doesn't change given prompt
*/
void splitPromptToCommandAndArguments(char* prompt, char** pCommand, char** pArguments)
{
	char *command = (char *)ver_malloc(sizeof(char) * (strlen(prompt) + 1));
	strcpy(command, prompt);

	if (prompt[0] == '!') // parse prompt regarding running previous commands: !!, !<num>, !<num>^str1^str2
		splitCommandAndArgumentsByToken(command, pArguments, '^');
	else // parse prompt regarding commands: find-apt, add-apt, buy-apt, delete-apt, history, short_history
		splitCommandAndArgumentsByToken(command, pArguments, ' ');
	*pCommand = command;
}

/*
* Split a command string by token delimiter. 
* If token is found, pArguments output parameter is set to the string's remaining arguments after token.
* Otherwise it's set to NULL
*/
void splitCommandAndArgumentsByToken(char* command, char** pArguments, char token)
{
	char *index = strchr(command, token);
	if (index == NULL)  // token not in command
		*pArguments = NULL;
	else // token found in command - split to arguments
	{
		*index = '\0';
		*pArguments = index + 1;
	}
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
			allocSize *= 2; // increase allocation size
			input = (char *)ver_realloc(input, allocSize * sizeof(char));
		}
		scanf("%c", &ch);
	}
	input[length++] = '\0';  // add finalizer string character
	input = (char *)ver_realloc(input, length * sizeof(char)); // re-allocate according to exact string length
	return input;
}
