/*
Filename: Utils.c
Authors: Poolitzer
Version: 0.1
Date: 16/06/2021
Shortdescription: This stores two utilities to calculate calender related values
*/
#include "Utils.h"

int dayofweekcalculator(int m, int d, int y) {
    /* This math row is copied from Mike Keith. He defined this formula in 
    1990 as the shortest way to figure out what day in the week a given day
    is. I need this to know where to start inserting the numbers in the 
    calendar. The explanation why this works can be found on his website: 
    http://www.cadaeic.net/calendar.htm */
    int D = (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / \
        100 + y / 400) % 7;
    // he defined Sunday as 0, and I want Monday as 0, so I substract one
    D--;
    // Sunday gets -1, we dont want that, we want it to be 6
    if (D == -1)
        D = 6;
    return D;
}

// this leap year calculation returns 1 for a leapyear and 0 for a not leapyear
// we had this implementation in a task and it doesn't get shorter than this
int leap_year(int year) {
    if (year % 400 == 0)
        return 1;
    else if (year % 100 == 0)
        return 0;
    else if (year % 4 == 0)
        return 1;
    else
        return 0;
}

