#include "stringUtils.h"
#define BUFFER_SIZE 50

void splitPromptToCommandAndArguments(char* prompt, char** pCommand, char** pArguments)
{
	char* command = (char*)ver_malloc(sizeof(char) * (strlen(prompt) + 1));
	strcpy(command, prompt);

	if (prompt[0] == '!') // parse prompt regarding running previous commands: !!, !<num>, !<num>^str1^str2
		splitCommandAndArgumentsByToken(command, pArguments, '^');
	else // parse prompt regarding commands: find-apt, add-apt, buy-apt, delete-apt, history, short_history
		splitCommandAndArgumentsByToken(command, pArguments, ' ');
	*pCommand = command;
}
void splitCommandAndArgumentsByToken(char* command, char** pArguments, char token)
{
	char* index = strchr(command, token);
	if (index == NULL)  // token not in command
		*pArguments = NULL;
	else // token found in command - split to arguments
	{
		*index = '\0';
		*pArguments = index + 1;
	}
}
char* strReplace(char* orig, char* str1, char* str2)
{
	char* result, * resultInsertPoint, * tmp;
	int replaceCount = countNumberOfReplacementStrings(orig, str1);
	size_t str1_len = strlen(str1);
	size_t str2_len = strlen(str2);
	size_t replaceSize = strlen(orig) + ((str2_len - str1_len) * replaceCount) + 1;
	resultInsertPoint = result = (char*)ver_malloc(sizeof(char) * replaceSize);

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
int countNumberOfReplacementStrings(char* str, char* repl)
{
	int count;
	size_t repl_len = strlen(repl);
	char* tmp;
	for (count = 0; tmp = strstr(str, repl); count++)
		str = tmp + repl_len;
	return count;
}

char* getInput()
{
	unsigned int allocSize = BUFFER_SIZE, length = 0;
	char* input;
	char ch;
	input = (char*)ver_malloc(allocSize * sizeof(char));
	// get input in chunks until the end ('\n' is given)
	scanf("%c", &ch); // get first character
	while (ch != '\n')
	{
		input[length++] = ch;
		if (length == allocSize)
		{
			allocSize *= 2; // increase allocation size
			input = (char*)ver_realloc(input, allocSize * sizeof(char));
		}
		scanf("%c", &ch);
	}
	input[length++] = '\0';  // add finalizer string character
	input = (char*)ver_realloc(input, length * sizeof(char)); // re-allocate according to exact string length
	return input;
}


