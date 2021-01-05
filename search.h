#ifndef __SEARCH_H
#define __SEARCH_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "memoryUtils.h"
#include "apartment.h"

#define MAX_PRICE "MaxPrice"
#define MIN_PRICE "MinPrice"
#define MIN_NUMBER_OF_ROOMS "MinNumRooms"
#define MAX_NUMBER_OF_ROOMS "MaxNumRooms"
#define DATE "Date"
#define ENTER "Enter"
#define SORTED_BY_THE_LOW_PRICE "s"
#define SORTED_BY_THE_HIGHEST_PRICE "sr"



/*** Search STRUCT FUNCTIONS ***/

//void printListByTheHighestPrice(ApartmentList* lst);

/*
*Update the apartment list with all the apartments cost up to NIS price
*/
void MaxPrice(int price, ApartmentList *lst);
/*
*Update the apartment list with all the apartments which starts at NIS price
*/
void MinPrice(int price, ApartmentList *lst);
/*
*Update the apartment list with all the apartments have at least numOfRooms
*/
void minNumOfRooms(int numOfRooms, ApartmentList *lst);
/*
*Update the apartment list with all the apartments with a maximum of numOfRooms
*/
void maxNumOfRooms(int numOfRooms, ApartmentList *lst);
/*
*Update the apartment list with all the apartments Which are vacant until the date
*/
void searchByDate(int date, ApartmentList *lst);
/*
*Update the apartment list with all the apartments that entered to the db in the last numDays
*/
void searchBEntryDate(int numDays, ApartmentList *lst);

/*
*Split the argument to the correct command according to the input
*/
void splitToCommands(char *arguments, ApartmentList *lst, int num,char* prevCommand);

/*
*Prints the expected data according to the find commands
*/
void findApt(char *arguments, ApartmentList *lst);


#endif