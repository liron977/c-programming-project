#include "search.h"
#include "apartment.h"


void MaxPrice(int price, ApartmentList *lst)
{
	ApartmentNode *curr = lst->head;
	ApartmentNode *currNext;
	while (curr != NULL)
	{
		currNext = curr->next;
		if (curr->apt->price > price)
		{
			removeApartmentFromListById(lst, curr->apt->id);
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
			removeApartmentFromListById(lst, curr->apt->id);
		}
		curr = curr->next;
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

			removeApartmentFromListById(lst, curr->apt->id);

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
			removeApartmentFromListById(lst, curr->apt->id);
		}
		curr = currNext;
	}
}
void searchByDate(int date, ApartmentList *lst)
{
	ApartmentNode *curr = lst->head;
	ApartmentNode *currNext;
	int yearDate, monthDate, dayDate;
	yearDate = date % 10000;
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
			removeApartmentFromListById(lst, apt->id);
		else if (apt->entryYear == yearDate) {
			if (apt->entryMonth > monthDate)
				removeApartmentFromListById(lst, apt->id);
			else if (apt->entryMonth == monthDate) {
				if (apt->entryDay >= dayDate)
					removeApartmentFromListById(lst, apt->id);
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
void splitToCommands(char *arguments, ApartmentList *lst, int num)
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
		printListByThelowPrice(lst);
	else if (strcmp(arguments, SORTED_BY_THE_HIGHEST_PRICE) == 0)
		printListByTheHighestPrice(lst);


}

void findApt(char *arguments, ApartmentList *lst)
{
	arguments = removeFirstSignal(arguments);
	char *token;
	char *prevToken = (char *)ver_malloc(sizeof(char) * (strlen(arguments) + 1));
	char *tmp;
	ApartmentList newAptLst;
	copyAptList(lst, &newAptLst);
	int num, count = 0;
	const char s[2] = "-";
	token = strtok(arguments, s);
	while (token != NULL) {  /* walk through other tokens */
		splitApartmentDetails(token, &tmp, ' ');
		if (tmp != NULL)
			sscanf(tmp, " %d", &num);
		strcpy(prevToken, token);
		count++;
		splitToCommands(token, &newAptLst, num);
		//sscanf(token, "%d", &num);
		token = strtok(NULL, s);
	}
	if ((count == 1) && (prevToken == ENTER)) {
		printListByApartmentCode(&newAptLst);
	}
	else if ((strcmp(prevToken, SORTED_BY_THE_HIGHEST_PRICE) == 0) && (strcmp(prevToken, SORTED_BY_THE_LOW_PRICE) == 0))
	{
		printApartmentList(&newAptLst);
	}
}
