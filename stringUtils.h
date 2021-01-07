#ifndef __STRINGUTILS_H
#define __STRINGUTILS_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUtils.h"



/*
* Splits a given prompt to it's command and arguments.
* Puts the result in output paramteres pCommand and pArguments.addApt
* If the prompt is only a command, pArguments is set to NULL.
* The function allocates memory for the command and arguments, and doesn't change given prompt
*/
void splitPromptToCommandAndArguments(char* prompt, char** pCommand, char** pArguments);

/*
*Split a command string by token delimiter.
* If token is found, pArguments output parameter is set to the string's remaining arguments after token.
* Otherwise it's set to NULL
*/
void splitCommandAndArgumentsByToken(char* command, char** pArguments, char token);

/*
* Replace all instances of str1 with str2 in orig string
* Doesn't change orig string. Allocates memory for new replaced string and returns it.
* Returned string should be freed by user
*/
char* strReplace(char* orig, char* str1, char* str2);

/*
* Counts and returns the number of instances of repl in str.
*/
int countNumberOfReplacementStrings(char* str, char* repl);

/*
* Reads a line of unknown length from stdin, until a new-line is given (\n char).
* Returns the line as a string
*/
char* getInput();

#endif()