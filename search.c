#include "search.h"
#include "apartment.h"
#include "stringUtils.h"


void MaxPrice(int price, ApartmentList *lst)
{
	ApartmentNode *curr = lst->head;
	ApartmentNode *currNext;
	while (curr != NULL)
	{
		currNext = curr->next;
		if (curr->apt->price > price)
		{
			removeApartmentNodeFromList(lst, curr);
		}
		curr = currNext;
	}
}
void MinPrice(int price, ApartmentList *lst)
{
	ApartmentNode *curr = lst->head;
	ApartmentNode *currNext;
	while (curr != NULL)
	{
		currNext = curr->next;
		if (curr->apt->price < price)
		{
			removeApartmentNodeFromList(lst, curr);	
		}
		curr = currNext;
	}
}
void minNumOfRooms(int numOfRooms, ApartmentList *lst)
{
	ApartmentNode *curr = lst->head;
	ApartmentNode *currNext;
	while (curr != NULL)
	{
		currNext = curr->next;
		if (curr->apt->numRooms < numOfRooms)
		{
			removeApartmentNodeFromList(lst, curr);
		}
		curr = currNext;
	}
}
void maxNumOfRooms(int numOfRooms, ApartmentList *lst)
{
	ApartmentNode *curr = lst->head;
	ApartmentNode *currNext;
	while (curr != NULL)
	{
		currNext = curr->next;
		if (curr->apt->numRooms > numOfRooms)
		{
			removeApartmentNodeFromList(lst, curr);
		}
		curr = currNext;
	}
}
void searchByDate(int date, ApartmentList *lst)
{
	ApartmentNode *curr = lst->head;
	ApartmentNode *currNext;
	int yearDate, monthDate, dayDate;
	//Split the date to year,month,day
	yearDate = date % 10000;
	yearDate = date % 2000;
	date = date / 10000;
	monthDate = date % 100;
	date = date / 100;
	dayDate = date % 100;
	date = date / 10;
	Apartment *apt;
	while (curr != NULL)
	{
		apt = curr->apt;
		currNext = curr->next;
		if (apt->entryYear > yearDate)
			removeApartmentNodeFromList(lst, curr);
		else if (apt->entryYear == yearDate) {
			if (apt->entryMonth > monthDate)
				removeApartmentNodeFromList(lst, curr);
			else if (apt->entryMonth == monthDate) {
				if (apt->entryDay >= dayDate)
					removeApartmentNodeFromList(lst, curr);
			}
		}
		curr = currNext;
	}
}
void searchBEntryDate(int numDays, ApartmentList *lst)
{
	double diffInSeconds = (double)(numDays) * (double)(24 * 60 * 60); // days * hours * minuts * seconds
	time_t currTime;
	time(&currTime);
	ApartmentNode *curr = lst->head, *tmp;
	while (curr != NULL)
	{
		tmp = curr->next;
		if (difftime(currTime, curr->apt->dbEntryDate) > diffInSeconds)
			removeApartmentNodeFromList(lst, curr);
		curr = tmp;
	}
}
void splitToCommands(char *arguments, ApartmentList *lst, int num,char* prevCommand)
{

	if (strcmp(arguments, MAX_PRICE) == 0)
		MaxPrice(num, lst);
	else if (strcmp(arguments, MIN_PRICE) == 0)
		MinPrice(num, lst);
	else if (strcmp(arguments, MIN_NUMBER_OF_ROOMS) == 0)
		minNumOfRooms(num, lst);
	else if (strcmp(arguments, MAX_NUMBER_OF_ROOMS) == 0)
		maxNumOfRooms(num, lst);
	else if (strcmp(arguments, DATE) == 0)
		searchByDate(num, lst);
	else if (strcmp(arguments, ENTER) == 0)
		searchBEntryDate(num, lst);
	else if (strcmp(arguments, SORTED_BY_THE_LOW_PRICE) == 0)
	{
		if (strcmp(prevCommand, ENTER) == 0)
			printListByApartmentCode(lst);
		else
			printApartmentList(lst);
	}
	else if (strcmp(arguments, SORTED_BY_THE_HIGHEST_PRICE) == 0)
	{
		if (strcmp(prevCommand, ENTER) == 0)
			printApartmentCodeByTheHighestPrice(lst);
		else
			printListByTheHighestPrice(lst);
	}
		
}
void findApt(char *arguments, ApartmentList *lst)
{
	arguments++;//Remove the first signal of the string 
	char* token;
	char* tmp ;
	char *prevToken = (char *)ver_malloc(sizeof(char) * (strlen(arguments) + 1));

	ApartmentList newAptLst;
	copyAptList(lst, &newAptLst);//creating a new list
	int num, count = 0;
	const char s[2] = "-";
	token = strtok(arguments, s);
	while (token != NULL) {  // walk through other tokens
		splitCommandAndArgumentsByToken(token, &tmp, ' ');//
		if (tmp != NULL)//if the command including numbers 
			sscanf(tmp, " %d", &num);//Receiving the numbers 
		count++;//In order to check if we got more than one of the print command
		splitToCommands(token, &newAptLst, num,prevToken);
		strcpy(prevToken, token);//Save the previous command
		token = strtok(NULL, s);
	}
	//if we have only 1 print command and the command is enter 
	if ((count == 1) && (strcmp(prevToken,ENTER)==0)){
		printListByApartmentCode(&newAptLst);
	}
	///The defult print command
	else if ((strcmp(prevToken, SORTED_BY_THE_HIGHEST_PRICE) != 0) && (strcmp(prevToken, SORTED_BY_THE_LOW_PRICE) != 0))
		printApartmentList(&newAptLst);
	freeApartmentList(&newAptLst);
	free(prevToken);
}
