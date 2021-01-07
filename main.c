#define _CRT_SECURE_NO_WARNINGS
/*
Student names: Liron Kotev, Idan Cohen
IDs: 318598380, 204363451

Course's final project - console application to manage real estate apartments
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUtils.h"
#include "apartment.h"
#include "history.h"
#include "apartment.h"
#include "search.h"
#include "stringUtils.h"

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

void runLastCommandFromHistory(ApartmentList *lst, History *historyDB);
void runCommandNumberFromHistory(ApartmentList *lst, char *command, char *arguments, History *historyDB);
void runApartmentCommandFromHistory(ApartmentList *lst, History *historyDB, char *prompt);
void runApartmentCommands(ApartmentList *lst, char *command, char *arguments, History *historyDB, char *prompt);

void main()
{
	ApartmentList aptsList;
	History historyDB;
	makeEmptyApartmentList(&aptsList);
	makeEmptyHistory(&historyDB);
	//read appartemnt from fille ,read history from file
	//readApartmentsFromBinaryFile(&aptsList); // TODO implement
	readHistoryFromTextFile(&historyDB, "history.txt");

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
			runLastCommandFromHistory(&aptsList, &historyDB);
		else if (command[0] == '!') // one of: !<num>, !<num>^str1^str2
			runCommandNumberFromHistory(&aptsList, command, arguments, &historyDB);
		else  // command is one of: find-apt, add-apt, buy-apt, delete-apt
			runApartmentCommands(&aptsList, command, arguments, &historyDB, input);
		free(command); // this also frees arguments memory
		free(input); // current input no longer needed
		input = getInput();  // get next prompt
	}
	// end of program
	writeHistoryToTextFile(&historyDB, "history.txt");
	//writeToApartmentsBinaryFile(); // TODO implement
	freeApartmentList(&aptsList);
	freeHistory(&historyDB);
	free(input); // free last exit command given
	puts("Good Bye!");
}

/*
* Gets the last prompt entered into a history database. If found, runs the prompt
*/
void runLastCommandFromHistory(ApartmentList *lst, History *historyDB)
{
	char *lastPrompt = getLastPrompt(historyDB);
	if (lastPrompt != NULL)  // last prompt found
		runApartmentCommandFromHistory(lst, historyDB, lastPrompt);
}

/*
* Runs the prompt in history with the given number from the command.
* Used for original commands: !<num>, !<num>^str1^str2
* command string should be !<num>
* arguments should be NULL if original command is !<num>, or str1^str2 if original command is !<num>^str1^str2
*/
void runCommandNumberFromHistory(ApartmentList *lst, char *command, char *arguments, History *historyDB)
{
	int promptNumber;
	sscanf(command + 1, "%d", &promptNumber); // get num after !
	char *prompt = getPromptNumber(historyDB, promptNumber);
	if (prompt != NULL) // prompt with this number found
	{
		if (arguments == NULL) // command case: !<num>. Just run the prompt found
			runApartmentCommandFromHistory(lst, historyDB, prompt);
		else // command case: !<num>^str1^str2. Replace string then run prompt
		{
			char *str1 = arguments, *str2, *replacePrompt;
			// arguments is str1^str2, split to two strings
			splitCommandAndArgumentsByToken(str1, &str2, '^');
			// New allocated string is returned since we don't want to change prompt directly (as its stored in history)
			replacePrompt = strReplace(prompt, str1, str2);
			runApartmentCommandFromHistory(lst, historyDB, replacePrompt);
			free(replacePrompt); // replaced prompt is like new input - needs to be freed after use
		}
	}
}

/*
* Runs an apartment command retrieved from history.
* Splits the prompt, taken from history, into the command and arguments and runs the needed command according to arguments
*/
void runApartmentCommandFromHistory(ApartmentList *lst, History *historyDB, char *prompt)
{
	char *command, *arguments;
	splitPromptToCommandAndArguments(prompt, &command, &arguments);
	runApartmentCommands(lst, command, arguments, historyDB, prompt);  // only apartments commands are kept in history
	free(command);
}

/*
* Runs a given apartment command according to arguments. Adds the prompt to the given history database
* Valid aparetment commands are: find-apt, add-apt, buy-apt, delete-apt
*/
void runApartmentCommands(ApartmentList *lst, char *command, char *arguments, History *historyDB, char *prompt)
{
	//printf(">> %s %s\n", command, arguments)//TO DO IN THE FILE
	if (strcmp(command, FIND_APT) == 0)
		findApt(arguments, lst);
	else if (strcmp(command, ADD_APT) == 0)
		addApt(arguments, lst);
	else if (strcmp(command, BUY_APT) == 0)
		buyApt(arguments, lst);
	else if (strcmp(command, DELETE_APT) == 0)
		deleteApt(arguments, lst);
	else // invalid apartment command, nothing to do
		return;
	addPromptToHistoryDatabase(historyDB, prompt);
}

