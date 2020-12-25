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
#include "apartment.h"

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

void runLastCommandFromHistory(History *historyDB);
void runCommandNumberFromHistory(char *command, char *arguments, History *historyDB);
void runApartmentCommandFromHistory(History *historyDB, char *prompt);
void runApartmentCommands(char *command, char *arguments, History *historyDB, char *prompt);
void splitPromptToCommandAndArguments(char *prompt, char **pCommand, char **pArguments);
void splitCommandAndArgumentsByToken(char *command, char **pArguments, char token);
char* strReplace(char *orig, char *str1, char *str2);
int countNumberOfReplacementStrings(char *str, char *repl);
char* getInput();


void main()
{
	ApartmentList aptsList;
	History historyDB;
	//makeEmptyApartmentList(&aptsList);
	//makeEmptyHistory(&historyDB);
	//read appartemnt from fille ,read history from file
	//readApartmentsFromBinaryFile(&aptsList); // TODO implement
	//readCommandHistoryFromFile(&historyDB); // TODO implement

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
		if (strcmp(command, HISTORY) == 0)
			printHistory(&historyDB);
		else if (strcmp(command, SHORT_HISTORY) == 0)
			printShortTermHistory(&historyDB);
		else if (strcmp(command, LAST_COMMAND) == 0)
			runLastCommandFromHistory(&historyDB);
		else if (command[0] == '!') // one of: !<num>, !<num>^str1^str2
			runCommandNumberFromHistory(command, arguments, &historyDB);
		else  // command is one of: find-apt, add-apt, buy-apt, delete-apt
			runApartmentCommands(command, arguments, &historyDB, input);
		free(command); // this also frees arguments memory
		free(input); // current input no longer needed
		input = getInput();  // get next prompt
	}
	// end of program
	writeToPromtsTextFile(); // TODO implement
	writeToApartmentsBinaryFile(); // TODO implement
	freeApartmentList(&aptsList);
	freeHistory(&historyDB);
	free(input); // free last exit command given
	puts("Good Bye!");
}

/*
* Gets the last prompt entered into a history database. If found, runs the prompt
*/
void runLastCommandFromHistory(History *historyDB)
{
	char *lastPrompt = getLastPrompt(historyDB);
	if (lastPrompt != NULL)  // last prompt found
		runApartmentCommandFromHistory(historyDB, lastPrompt);
}

/*
* Runs the prompt in history with the given number from the command.
* Used for original commands: !<num>, !<num>^str1^str2
* command string should be !<num>
* arguments should be NULL if original command is !<num>, or str1^str2 if original command is !<num>^str1^str2
*/
void runCommandNumberFromHistory(char *command, char *arguments, History *historyDB)
{
	int promptNumber;
	sscanf(command + 1, "%d", &promptNumber); // get num after !
	char *prompt = getPromptNumber(historyDB, promptNumber);
	if (prompt != NULL) // prompt with this number found
	{
		if (arguments == NULL) // command case: !<num>. Just run the prompt found
			runApartmentCommandFromHistory(historyDB, prompt);
		else // command case: !<num>^str1^str2. Replace string then run prompt
		{
			char *str1 = arguments, *str2, *replacePrompt;
			// arguments is str1^str2, split to two strings
			splitCommandAndArgumentsByToken(str1, &str2, '^');
			// New allocated string is returned since we don't want to change prompt directly (as its stored in history)
			replacePrompt = strReplace(prompt, str1, str2);
			runApartmentCommandFromHistory(historyDB, replacePrompt);
			free(replacePrompt); // replaced prompt is like new input - needs to be freed after use
		}
	}
}

/*
* Runs an apartment command retrieved from history.
* Splits the prompt, taken from history, into the command and arguments and runs the needed command according to arguments
*/
void runApartmentCommandFromHistory(History *historyDB, char *prompt)
{
	char *command, *arguments;
	splitPromptToCommandAndArguments(prompt, &command, &arguments);
	runApartmentCommands(command, arguments, historyDB, prompt);  // only apartments commands are kept in history
	free(command);
}

/*
* Runs a given apartment command according to arguments. Adds the prompt to the given history database
* Valid aparetment commands are: find-apt, add-apt, buy-apt, delete-apt
*/
void runApartmentCommands(char *command, char *arguments, History *historyDB, char *prompt)
{
	if (strcmp(command, FIND_APT) == 0)
		findApt(arguments); // TODO implement
	else if (strcmp(command, ADD_APT) == 0)
		addApt(arguments); // TODO implement
	else if (strcmp(command, BUY_APT) == 0)
		buyApt(arguments); // TODO implement
	else if (strcmp(command, DELETE_APT) == 0)
		deleteApt(arguments); // TODO implement
	else // invalid apartment command, nothing to do
		return;
	addPromptToHistoryDatabase(historyDB, prompt);
}

/*
* Splits a given prompt to it's command and arguments.
* Puts the result in output paramteres pCommand and pArguments.
* If the prompt is only a command, pArguments is set to NULL.
* The function allocates memory for the command and arguments, and doesn't change given prompt
*/
void splitPromptToCommandAndArguments(char *prompt, char **pCommand, char **pArguments)
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
void splitCommandAndArgumentsByToken(char *command, char **pArguments, char token)
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
* Replace all instances of str1 with str2 in orig string
* Doesn't change orig string. Allocates memory for new replaced string and returns it.
* Returned string should be freed by user
*/
char* strReplace(char *orig, char *str1, char *str2)
{
	char *result, *resultInsertPoint, *tmp;
	int replaceCount = countNumberOfReplacementStrings(orig, str1);
	size_t str1_len = strlen(str1);
	size_t str2_len = strlen(str2);
	size_t replaceSize = strlen(orig) + ((str2_len - str1_len) * replaceCount) + 1;
	resultInsertPoint = result = (char *)ver_malloc(sizeof(char) * replaceSize);

	while (replaceCount--)
	{
		tmp = strstr(orig, str1);

		// copy up to str1
		memcpy(resultInsertPoint, orig, tmp - orig);
		resultInsertPoint += tmp - orig;

		// copy replacement string
		memcpy(resultInsertPoint, str2, str2_len);
		resultInsertPoint += str2_len;

		// update orig string location
		orig = tmp + str1_len;
	}
	// all replacements copied - copy anything left after last replacement
	strcpy(resultInsertPoint, orig);
	return result;
}

/*
* Counts and returns the number of instances of repl in str.
*/
int countNumberOfReplacementStrings(char *str, char *repl)
{
	int count;
	size_t repl_len = strlen(repl);
	char *tmp;
	for (count = 0; tmp = strstr(str, repl); count++)
		str = tmp + repl_len;
	return count;
}

/*
* Reads a line of unknown length from stdin, until a new-line is given (\n char).
* Returns the line as a string
*/
char* getInput()
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
