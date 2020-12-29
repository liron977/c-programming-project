#include "search.h"


void MaxPrice(int price, ApartmentList* lst)
{
    ApartmentNode* curr = lst->head;
    while (curr != NULL)
    {
        if (curr->apt->price > price)
        {
            removeApartmentFromListById(lst, curr->apt->id);
        }
        curr = curr->next;
    }
}
void MinPrice(int price, ApartmentList* lst)
{
    ApartmentNode* curr = lst->head;
    while (curr != NULL)
    {
        if (curr->apt->price < price)
        {
            removeApartmentFromListById(lst, curr->apt->id);
        }
        curr = curr->next;
    }
}
void minNumOfRooms(int numOfRooms, ApartmentList* lst)
{
    ApartmentNode* curr = lst->head;
    while (curr != NULL)
    {
        if (curr->apt->numRooms < numOfRooms)
        {
            removeApartmentFromListById(lst, curr->apt->id);
        }
        curr = curr->next;
    }
}
void maxNumOfRooms(int numOfRooms, ApartmentList* lst)
{
    ApartmentNode* curr = lst->head;
    while (curr != NULL)
    {
        if (curr->apt->numRooms > numOfRooms)
        {
            removeApartmentFromListById(lst, curr->apt->id);
        }
        curr = curr->next;
    }
}
void searchByDate(int date, ApartmentList* lst)
{
    ApartmentNode* curr = lst->head;
    int yearDate, monthDate, dayDate;
    yearDate = date % 10000;
    date = date / 10000;
    monthDate = date % 100;
    date = date / 100;
    dayDate = date % 100;
    date = date / 10;
    Apartment* apt;
    while (curr != NULL)
    {
        apt = curr->apt;
        if(apt->entryYear > yearDate)
            removeApartmentFromListById(lst, apt->id);
        else if (apt->entryYear == yearDate){
            if(apt->entryMonth > monthDate)
                removeApartmentFromListById(lst, apt->id);
            else if (apt->entryMonth == monthDate){
                if(apt->entryDay >= dayDate)
                    removeApartmentFromListById(lst, apt->id);
            }
        }
    }

}
void searchBEntryDate(int numDays, ApartmentList* lst)
{
    double diffInSeconds = (double)(numDays) * (double)(24 * 60 * 60); // days * hours * minuts * seconds
    time_t currTime;
    time(&currTime);
    ApartmentNode* curr = lst->head, * tmp;
    while (curr != NULL)
    {
        tmp = curr->next;
        if (difftime(currTime, curr->apt->dbEntryDate) > diffInSeconds)
            removeApartmentNodeFromList(lst, curr);
        curr = tmp;
    }
}
void splitToCommands(char* arguments, ApartmentList* lst, int num)
{

    if (arguments == MAX_PRICE)
        MaxPrice(num, lst);
    else if (arguments == MIN_PRICE)
        MinPrice(num, lst);
    else if (arguments == MIN_NUMBER_OF_ROOMS)
        minNumOfRooms(num, lst);
    else if (arguments == MAX_NUMBER_OF_ROOMS)
        maxNumOfRooms(num, lst);
    else if (arguments == DATE)
        searchByDate(num, lst);
    else if (arguments == ENTER)
        searchBEntryDate(num, lst);
    else if (arguments == SORTED_BY_THE_LOW_PRICE)
        printListByThelowPrice(num, lst);
    else if (arguments == SORTED_BY_THE_HIGHEST_PRICE)
        printListByTheHighestPrice(num, lst);


}

void findApt(char* arguments, ApartmentList* lst)
{
    arguments = removeFirstSignal(arguments);
    char* token, tmp = " ", prevToken = (char*)malloc(sizeof(char) * sizeof(arguments));
    ApartmentList* newAptLst;
    copyAptList(lst, newAptLst);
    int num, count = 0;
    const char s[2] = "-";
    token = strtok(arguments, s);
    while (token != NULL) {  /* walk through other tokens */
        splitApartmentDetails(token, &tmp, ' ');
        sscanf(tmp, " %d", &num);
        strcpy(prevToken, token);
        count++;
        splitToCommands(token, newAptLst, num);
        sscanf(token, "%d", &num);
        token = strtok(NULL, s);
    }
    if ((count == 1) && (prevToken == ENTER)) {
        printListByApartmentCode(newAptLst);
    }
    else if ((prevToken != SORTED_BY_THE_HIGHEST_PRICE) && (prevToken != SORTED_BY_THE_LOW_PRICE))
    {
        printApartmentList(newAptLst);
    }
}
