#include "apartment.h"

Apartment* createNewApartment(unsigned int id, char *address, int price, short int numRoom, short int entryDay, short int entryMonth, short int entryYear)
{
	Apartment *newApt = (Apartment *)ver_malloc(sizeof(Apartment));
	newApt->id = id;
	newApt->address = address;
	newApt->price = price;
	newApt->numRooms = numRoom;
	newApt->entryDay = entryDay;
	newApt->entryMonth = entryMonth;
	newApt->entryYear = entryYear;
	newApt->dbEntryDate = time(NULL);
	return newApt;
}

void printApartment(Apartment *apt)
{
	// get database entry date as string in format: xx.xx.xxxx , for example: 12.07.2020
	char dbEntryDate[DB_ENTRY_MAX_STRING_LENGTH];
	struct tm *dbEntryInfo = localtime(&(apt->dbEntryDate));
	strftime(dbEntryDate, DB_ENTRY_MAX_STRING_LENGTH, "%d.%m.%Y", dbEntryInfo);

	// print an apartment according to instructions
	puts("Apt details:");
	printf("Code: %u\n", apt->id);
	printf("Address: %s\n", apt->address);
	printf("Number of rooms: %hd\n", apt->numRooms);
	printf("Price: %d\n", apt->price);
	printf("Entry date: %hd.%hd.%hd\n", apt->entryDay, apt->entryMonth, apt->entryYear);
	printf("Database entry date: %s\n", dbEntryDate);
}

void freeApartment(Apartment *apt)
{
	free(apt->address);
	free(apt);
}

void makeEmptyApartmentList(ApartmentList *lst)
{
	lst->head = lst->tail = NULL;
}

bool isEmptyApartmentList(ApartmentList *lst)
{
	return lst->head == NULL;
}

ApartmentNode* createNewApartmentListNode(Apartment *apt, ApartmentNode *next, ApartmentNode *prev)
{
	ApartmentNode *res = (ApartmentNode *)ver_malloc(sizeof(ApartmentNode));
	res->apt = apt;
	res->next = next;
	res->prev = prev;
	return res;
}

void insertApartmentToList(ApartmentList *lst, Apartment *apt)
{
	ApartmentNode *newAptNode, *after;
	newAptNode = createNewApartmentListNode(apt, NULL, NULL);
	after = getApartmentNodeBefore(lst, apt->price);
	insertApartmentNodeToList(lst, newAptNode, after);
}

void insertApartmentNodeToList(ApartmentList *lst, ApartmentNode *node, ApartmentNode *after)
{
	if (isEmptyApartmentList(lst) || after == NULL)  // start of list
		insertApartmentNodeToStartList(lst, node);
	else if (after->next == NULL)  // end of list
		insertApartmentNodeToEndList(lst, node);
	else  // middle of list
	{
		node->next = after->next;
		node->prev = after;
		after->next->prev = node;
		after->next = node;
	}
}

void insertApartmentNodeToEndList(ApartmentList *lst, ApartmentNode *tail)
{
	if (isEmptyApartmentList(lst))
	{
		tail->prev = NULL;
		lst->head = lst->tail = tail;
	}
	else
	{
		tail->prev = lst->tail;
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
}

void insertApartmentNodeToStartList(ApartmentList *lst, ApartmentNode *head)
{
	if (isEmptyApartmentList(lst))
	{
		head->next = NULL;
		lst->head = lst->tail = head;
	}
	else
	{
		head->next = lst->head;
		lst->head->prev = head;
		lst->head = head;
	}
	head->prev = NULL;
}

ApartmentNode* getApartmentNodeBefore(ApartmentList *lst, int price)
{
	if (isEmptyApartmentList(lst) || price <= lst->head->apt->price)  // price is smaller than minimum in list - should be new head
		return NULL;
	else if (price >= lst->tail->apt->price)  // price is greater than maximum in list. Return the current maximum (tail)
		return lst->tail;
	else  // list_min < price < list_max - find the first "middle" apartment where price < apt->price
	{
		ApartmentNode *curr = lst->head;
		// skip over all apartments that have lower prices
		while (curr != NULL && curr->apt->price < price)
			curr = curr->next;
		return curr->prev; // note: curr can't be null here, so its valid to access prev
	}
}

void removeApartmentFromListById(ApartmentList *lst, unsigned int id)
{
	ApartmentNode *curr = lst->head;
	while (curr != NULL && curr->apt->id != id) // get the apartment with given id
		curr = curr->next;
	if (curr != NULL) // apartment with this id found, remove it from list
		removeApartmentNodeFromList(lst, curr);
}

void removeApartmentsFromListByEntryDate(ApartmentList *lst, int numDays)
{
	double diffInSeconds = (double)(numDays * 24 * 60 * 60); // days * hours * minuts * seconds
	time_t currTime;
	time(&currTime);

	ApartmentNode *curr = lst->head, *tmp;
	while (curr != NULL)
	{
		tmp = curr->next;
		if (difftime(currTime, curr->apt->dbEntryDate) <= diffInSeconds)
			removeApartmentNodeFromList(lst, curr);
		curr = tmp;
	}
}

void removeApartmentNodeFromList(ApartmentList *lst, ApartmentNode *node)
{
	if (isEmptyApartmentList(lst)) // empty list, nothing to remove
		return;
	else if (lst->head == lst->tail)  // list length is 1 (only 1 apartment in list). Change to empty list
		makeEmptyApartmentList(lst);
	else if (lst->head == node)  // remove from start of list
	{
		node->next->prev = node->prev;
		lst->head = node->next;
	}
	else if (lst->tail == node)  // remove from end of list
	{
		node->prev->next = node->next;
		lst->tail = node->prev;
	}
	else  // remove from middle of list
	{
		node->next->prev = node->prev;
		node->prev->next = node->next;
	}
	freeApartmentNode(node);
}

void freeApartmentList(ApartmentList *lst)
{
	ApartmentNode *curr = lst->head, *tmp;
	while (curr != NULL)
	{
		tmp = curr->next;
		freeApartmentNode(curr);
		curr = tmp;
	}
}

void freeApartmentNode(ApartmentNode *node)
{
	freeApartment(node->apt);
	free(node);
}
