#include "apartment.h"
short int apartmentCode = 1;

void addApt(char *arguments, ApartmentList *lst)
{
	char *details;
	int price;
	short int numRooms, entryDay, entryMonth, entryYear;
	time_t dbEntryDate = time(NULL);;

	arguments++; //Remove the rirst signal
	splitCommandAndArgumentsByToken(arguments, &details, '\"'); //Split the argument
	char *apartmentAddress = (char *)ver_malloc(sizeof(char) * (strlen(arguments) + 1));
	strcpy(apartmentAddress, arguments);

	sscanf(details, "%d%hd%hd%hd%hd", &price, &numRooms, &entryDay, &entryMonth, &entryYear);
	
	Apartment *newApt = createNewApartment(apartmentCode, apartmentAddress, price, numRooms, entryDay, entryMonth, entryYear, dbEntryDate);
	insertApartmentToList(lst, newApt);
	apartmentCode++;
}

void deleteApt(char *arguments, ApartmentList *lst)
{
	int numDays;
	sscanf(arguments, "%*s %d", &numDays); // get the number from the string 
	removeApartmentsFromListByEntryDate(lst, numDays);
}

void buyApt(char *codeApt, ApartmentList *lst)
{
	short int num;
	sscanf(codeApt, "%hd", &num);//In order to get the code from the string 
	removeApartmentFromListById(lst, num);
}

Apartment *createNewApartment(short int id, char *address, int price, short int numRoom, short int entryDay, short int entryMonth, short int entryYear, time_t dbEntryDate)
{
	Apartment *newApt = (Apartment *)ver_malloc(sizeof(Apartment));
	newApt->id = id;
	newApt->address = address;
	newApt->price = price;
	newApt->numRooms = numRoom;
	newApt->entryDay = entryDay;
	newApt->entryMonth = entryMonth;
	newApt->entryYear = entryYear;
	newApt->dbEntryDate = dbEntryDate;
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
	printf("Code: %hd\n", apt->id);
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
	lst->size = 0;
}

bool isEmptyApartmentList(ApartmentList *lst)
{
	return lst->head == NULL;
}

void copyAptList(ApartmentList *lst, ApartmentList *newLst)
{

	makeEmptyApartmentList(newLst);
	ApartmentNode *curr = lst->head;
	Apartment *aptNewList;
	Apartment *tmp;
	while (curr != NULL) {
		tmp = curr->apt;
		char *newAddress = (char *)ver_malloc(sizeof(char) * strlen(tmp->address) + 1);
		strcpy(newAddress, tmp->address);
		aptNewList = createNewApartment(tmp->id, newAddress, tmp->price, tmp->numRooms, tmp->entryDay, tmp->entryMonth, tmp->entryYear, tmp->dbEntryDate);
		insertApartmentToList(newLst, aptNewList);
		curr = curr->next;
	}
}

ApartmentNode *createNewApartmentListNode(Apartment *apt, ApartmentNode *next, ApartmentNode *prev)
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
	lst->size = lst->size + 1;
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

ApartmentNode *getApartmentNodeBefore(ApartmentList *lst, int price)
{
	if (isEmptyApartmentList(lst))
		return NULL;
	else if (price < lst->head->apt->price)  // price is smaller than minimum in list - should be new head
		return NULL;
	else if (price >= lst->tail->apt->price)  // price is greater than maximum in list. Return the current maximum (tail)
		return lst->tail;
	else  // list_min <= price < list_max - find the first "middle" apartment where price < apt->price
	{
		ApartmentNode *curr = lst->head;
		// skip over all apartments that have lower prices
		while (curr != NULL && curr->apt->price <= price)
			curr = curr->next;
		return curr->prev; // note: curr can't be null here, so its valid to access prev
	}
}

void removeApartmentFromListById(ApartmentList *lst, short int id)
{
	ApartmentNode *curr = lst->head;
	while (curr != NULL && curr->apt->id != id) // get the apartment with given id
		curr = curr->next;
	if (curr != NULL) // apartment with this id found, remove it from list
		removeApartmentNodeFromList(lst, curr);
	else
	{
		puts("Sorry, this apartment id does not exist");
	}
}

void removeApartmentsFromListByEntryDate(ApartmentList *lst, int numDays)
{
	double diffInSeconds = (double)(numDays) * (double)(24 * 60 * 60); // days * hours * minuts * seconds
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
	lst->size = lst->size - 1;
}

void printApartmentList(ApartmentList *lst)
{
	ApartmentNode *curr = lst->head;
	while (curr != NULL)
	{
		printApartment(curr->apt);
		curr = curr->next;
	}
}

void printListByTheHighestPrice(ApartmentList *lst)
{
	ApartmentNode *curr = lst->tail;
	while (curr != NULL)
	{
		printApartment(curr->apt);
		curr = curr->prev;
	}
}

void printApartmentCodeByTheHighestPrice(ApartmentList *lst)
{
	ApartmentNode *curr = lst->tail;
	while (curr != NULL)
	{
		// print an apartment according to instructions
		puts("Apt details:");
		printf("Code: %hd\n", curr->apt->id);
		curr = curr->prev;
	}
}

void printListByApartmentCode(ApartmentList *lst)
{
	// print an apartment according to instructions
	ApartmentNode *curr = lst->head;
	while (curr != NULL)
	{
		puts("Apt details:");
		printf("Code: %hd\n", curr->apt->id);
		curr = curr->next;
	}

}

void readApartmentsFromBinaryFile(ApartmentList *lst, char *fname)
{
	short int id, addressLength, numRooms, entryDay, entryMonth, entryYear;
	short int highestId = 0; // list is sorted by price, not id. needed for global apt code variable to continue correctly giving ids
	int size, price, i;
	time_t dbEntryDate;
	char *address;
	BYTE compressedBitsArr[COMPRESSED_BITS_ARRAY_SIZE];
	Apartment *apt;

	FILE *fin = fopen(fname, "rb");
	if (!fin && errno == 2) // errno 2 is "No such file or directory" error - could happen if no previous runs occured or history was deleted
		return;
	verifyFileOpen(fin, fname);

	fread(&size, sizeof(size), 1, fin);

	for (i = 0; i < size; i++)
	{
		// read fields: id, address, price, db entry date
		fread(&id, sizeof(id), 1, fin);
		fread(&addressLength, sizeof(addressLength), 1, fin);
		address = (char *)ver_malloc(sizeof(char) * (addressLength + 1));
		fread(address, sizeof(char), addressLength, fin);
		address[addressLength] = '\0';
		fread(&price, sizeof(price), 1, fin);
		fread(&dbEntryDate, sizeof(dbEntryDate), 1, fin);

		// decompress fields num rooms and entry date (day/month/year) back from bits
		fread(compressedBitsArr, sizeof(BYTE), COMPRESSED_BITS_ARRAY_SIZE, fin);
		
		// decompress num rooms - abcd XXXX --> 0000 abcd
		numRooms = compressedBitsArr[0] >> 4;

		/*
		* decompress entry day - split between 2 bytes (XXXXabcd eXXXXXXX):
		* XXXX abcd --> 0000 abcd (mask = 0000 1111)
		* abcd --> abcd0
		* eXXXXXXX --> 0000000e
		* abcd0 | 0000e --> abcde
		*/
		entryDay = ((compressedBitsArr[0] & 0x0F) << 1) | (compressedBitsArr[1] >> 7);

		// decompress entry month - XabcdXXX --> 000Xabcd --> 0000abcd (mask = 0000 1111)
		entryMonth = (compressedBitsArr[1] >> 3) & 0x0F;
		
		/*
		* decompress entry year - split between 2 bytes (XXXXXabc defgXXXX):
		* XXXXXabc --> 00000abc (mask = 0000 0111)
		* 00000abc --> 0abc0000
		* defgXXXX --> 0000defg
		* 0abc0000 | 0000defg  --> 0abcdefg
		*/
		entryYear = ((compressedBitsArr[1] & 0x07) << 4) | (compressedBitsArr[2] >> 4);

		apt = createNewApartment(id, address, price, numRooms, entryDay, entryMonth, entryYear, dbEntryDate);
		insertApartmentToList(lst, apt);

		highestId = id > highestId ? id : highestId;
	}
	fclose(fin);
	apartmentCode = highestId + 1; // global counter initialized to next code, after last apartment in file
}

void writeApartmentsToBinaryFile(ApartmentList *lst, char *fname)
{
	if (lst->size == 0) // empty list - no apartments to write. nothing to do
		return;

	ApartmentNode *curr = lst->head;
	short int addressLength;

	FILE *fout = fopen(fname, "wb");
	verifyFileOpen(fout, fname);

	fwrite(&(lst->size), sizeof(lst->size), 1, fout); // write number of apartments

	while (curr != NULL)
	{
		// first write fields: id, address length + address (without \0), price, db entry date
		fwrite(&(curr->apt->id), sizeof(curr->apt->id), 1, fout);
		addressLength = strlen(curr->apt->address);
		fwrite(&addressLength, sizeof(addressLength), 1, fout);
		fwrite(curr->apt->address, sizeof(char), addressLength, fout);
		fwrite(&(curr->apt->price), sizeof(curr->apt->price), 1, fout);
		fwrite(&(curr->apt->dbEntryDate), sizeof(curr->apt->dbEntryDate), 1, fout);

		// now write num rooms and entry date (day/month/year) as compressed byte array
		writeCompressedApartmentFieldsToBinaryFile(fout, curr->apt);

		curr = curr->next;
	}
	fclose(fout);
}

void writeCompressedApartmentFieldsToBinaryFile(FILE *fout, Apartment *apt)
{
	BYTE compressedBitsArr[COMPRESSED_BITS_ARRAY_SIZE] = { 0 };

	// write compressed num rooms
	compressedBitsArr[0] |= (apt->numRooms << 4);

	// write compressed entry day - entry day is split between 2 bytes
	compressedBitsArr[0] |= (apt->entryDay >> 1);
	compressedBitsArr[1] |= (apt->entryDay << 7);

	// write compressed entry month
	compressedBitsArr[1] |= (apt->entryMonth << 3);

	// write compressed entry year - entry year is split between 2 bytes
	compressedBitsArr[1] |= (apt->entryYear >> 4);
	compressedBitsArr[2] |= (apt->entryYear << 4);

	fwrite(compressedBitsArr, sizeof(BYTE), COMPRESSED_BITS_ARRAY_SIZE, fout);
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
