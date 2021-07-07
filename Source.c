/*
Filename: Source.c
Authors: Poolitzer
Version: 0.1
Date: 16/06/2021
Shortdescription: This program shows an interactive calendar which allows to 
store appointments.
Longdescription: The main function is after an initial date picking phase a 
while loop, which allows navigating the calendar. There are two drawing related
functions which take care of generating the calendar, three function which 
store, show, and delete appointements and one utility which changes the day of
Februrary if its a leap year.
*/

#include <stdio.h>
#include <stdlib.h> //this import exists to do system("cls");
#include <string.h> //this import exists to do strlen(title);
#include "Utils.h" //dayofweekcalculator and leap_year;
// this is the max amount of appointments the program allows to be stored.
// the 10 is a very abritary number.
#define AMOUNT_DATES 10

//yes, this is a global variable, I am sorry. There is only one function in
// this code where a value could be changed though: leap_year_change. So it's
// very close to a constant.
int DAY_MONTHS[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; 

/* Block of functions, see below */

void print_date(int day, int day_counter, int day_with_date, \
    int date_days[AMOUNT_DATES]);

void month_drawer(int day, int month, int year, int date_storage[AMOUNT_DATES][3]);

/* Second block, see below */

void add_date(int day, int month, int year, int date_storage[AMOUNT_DATES][3],\
    char date_names[AMOUNT_DATES][50]);

void delete_date(int day, int month, int year, int date_storage[AMOUNT_DATES][3],\
    char date_titles[AMOUNT_DATES][50]);

void show_date(int day, int month, int year, int date_storage[AMOUNT_DATES][3],\
    char date_titles[AMOUNT_DATES][50]);

/* and the third*/

void leap_year_change(int year);

int main()
{
    char c; // char to store the single character input of the user
    int validation = 0; // return of scanf_s, or wrong input from the user
    int day = 31; 
    int month = 12; // these three are exactly what one think they are
    int year = 2020;
    // this is later used since the DAY_MONTHS array starts with 0
    int month_for_array = month - 1; 

    // these variables hold the appointments from the user. Both of them are
    // 2D arrays, the rows belong together. So the date for appointment 0 is
    // stored in _storage at index 0 with daz, month, year set as the columns.
    // the title to that is stord in _titles as char with up to 49 characters.
    int date_storage[AMOUNT_DATES][3] = { {0} }; 
    char date_titles[AMOUNT_DATES][50];
    

    printf("Hi. This is the first time you run this appliction. Welcome to the\
 interactive calendar, right in your console. How awesome is this? To start, \
you need to tell me the date you want to open the calendar at. Start with the \
day. Enter an integer between 1 and 31: ");

    // this while loop is active as long as the user didn't input anything or
    // their input is out of range
    while (validation == 0) {
        validation = scanf_s("%d", &day);
        // this if is true when either the input is wrong (no number given)
        // or when the number is too low/high
        if (validation != 1 || day < 1 || day > 31) {
            // if validation is 0, the input buffer exists, so I DESTROY it
            while ((c = getchar()) != '\n' && c != EOF);
            printf("\nSorry, this was the wrong input. You need to give an \
integer between 1 and 31. Try again: ");
            // validation could be one, and then the while loop exists, so we
            // set it to 0. Happens when day is too high or low
            validation = 0;
        }
    }

    // reset validation so we can reuse the while loop
    validation = 0;
    printf("\nWhat a number. Amazing. Lets do this again. Enter an integer \
between 1 and 12 for the, you guessed it, month: ");

    while (validation == 0) {
        validation = scanf_s("%d", &month);
        if (validation != 1 || month < 1 || month > 12) {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("\nSorry, this was the wrong input. You need to give an \
integer between 1 and 12. Try again: ");
            validation = 0;
        }
    }

    validation = 0;
    printf("\nNice! We are almost done with the setup, now I just need a \
year from you. Give me something between 1900 and, uhhh, 3000 I guess? \
Are you from the future? Anyway, hit me with it: ");
    while (validation == 0) {
        validation = scanf_s("%d", &year);
        if (validation != 1 || year < 1900 || year > 3000) {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("\nSorry, this was the wrong input. You need to give an\
integer between 1900 and 3000. Try again: ");
            validation = 0;
        }
    }

    printf("\n\nGreat! We are done! Use 'a' and 'd' to move around. Once you \
got comfortable with this, hit 'h' to get an explanation for the other \
keystrokes. This message will vanish once you do the first moving part.\n\n");
    
    // we clear the input buffer, then call the leap year change function, 
    // since the year the user put in might be one
    while ((c = getchar()) != '\n' && c != EOF);
    leap_year_change(year);

    // this is an infinite loop. I think calling a while loop with 1 is 
    // regarded bad practice, but every other way of achieving a "while loop
    // until specific keystroke" relied on more libraries and I need to get
    // the keystroke later in the code anyway. The break condition is called
    // in line 130, when the user inputs q
    while (1) {

        // this function draws/prints the selected month
        month_drawer(day, month, year, date_storage);

        validation = scanf_s("%c", &c, 1);

        // we dont need a while loop for this, because we are already in one
        if (validation == 0) {
            printf("Your input was invalid.. Please try again. \
You can use 'h' to obtain a help message. Press enter to continue.");

            // since validation is 0, the input buffer isn't cleared. Yet.
            while ((c = getchar()) != '\n' && c != EOF);
            // if we didn't wait for the user enter here, the code would 
            // instantly loop and clear the screen, such making this message
            // disappear before one can read it. Waiting for any input and then
            // continuing is way better
            scanf_s("%c", &c, 1);
            continue;
        }

        // since there is a valid input, we can switch to the appropriate
        // actions. Either modifying day/motnh or calling a function
        switch (c) {
            case 'h':
                printf("Help for this calendar:\n\nUse 'a' to go one day back \
and 'd' to move one forward.\n'w' and 's' can be used to jump one month up or \
down, respectively.\nHit 'g' to add an appointment to the day you selected.\
 You will then be interactively guided through the steps to actually add it. \
The days which have an appointment will have a * next to it.\nYou can hit 'f' \
to see the saved appointments for the selected day and 'r' to remove the \
appointment from that day.\nThen there is 'q' to quite the application and 'h' \
to get this help. To close this message, hit enter.");
                // again, if we didnt wait, the screen would be instantly 
                // cleared and no one could have read this message
                scanf_s("%c", &c, 1);
                break;
            case 'q':
                printf("Thanks for using this calendar, and goodbye!");
                return(0);
            case 'd':
                day++;
                break;
            case 'a':
                day--;
                break;
            case 'w':
                month--;
                break;
            case 's':
                month++;
                break;
            case 'g':
                add_date(day, month, year, date_storage, date_titles);
                break;
            case 'r':
                delete_date(day, month, year, date_storage, date_titles);
                break;
            case 'f':
                show_date(day, month, year, date_storage, date_titles);
                break;
            default:
                printf("Your input was invalid. Please try again. You can use \
'h' to obtain a help message. Press enter to continue.");
                scanf_s("%c", &c, 1);
        }

        // updating this variable with the potential new month
        month_for_array = month - 1;

        // this if clause is entered when either 'a' is pressed from the 1st Jan
        // or 'w' from anywhere in january
        if ((day <= 0 && month_for_array == 0) || month_for_array < 0) {
            // if its the month jump, we want to preserve the day, otherwise set
            // it to the last day in the year
            if (c != 'w')
                day = 31;
            month = 12;
            year = year - 1;
            // since the year changes, we call the potential change for february
            leap_year_change(year);
        }
        // this if clause is for 'd' after the 31st Dec or 's' in that month
        else if (day >= 32 && month_for_array == 11 || month_for_array > 11) {
            if (c != 's')
                day = 1;
            month = 1;
            year = year + 1;
            leap_year_change(year);
        }
        // this if clause is for a month jump. it can happen when either 'd' is
        // pressed or one of the month jump buttons
        else if (day > DAY_MONTHS[month_for_array]) {
            // this happens e.g. from Jan to Feb or March to Feb, when the 30
            // day in the months is selected and then 'w' or 's' is pressed
            if (c == 's' || c == 'w')
                day = DAY_MONTHS[month_for_array];
            // otherwise a normal jump with 'd' from the last day of the month
            // is attempted
            else{
                day = 1;
                month++;
            }
            
        }
        // this can only happen when 'a' is pressed on the first of a month
        else if (day <= 0) {
                month--;
                // minus one because the month is subtracted
                day = DAY_MONTHS[month_for_array - 1];
            }

        // this is neccessary because otherwise scanf_s reads something invalid
        // after the valid input
        while ((c = getchar()) != '\n' && c != EOF);
        
        // this clears the screen. Only on windows, but we will cross the cross
        // platform bridge when we come to it
        system("cls");
    }
    return(0);
}


// Utility which changes the Februrary of the global variable
void leap_year_change(int year) {
    // this calls the leap_year utility from the header. If it returns 1, the
    // year is a leap year, so we set the DAY_MONTHS variable (which is an
    // otherwise constant global variable) in the februrary place to the
    // appropriate day
    if (leap_year(year) == 1)
        DAY_MONTHS[1] = 29;
    else
        DAY_MONTHS[1] = 28;
}

/* Calendar drawer + the actual print function */

// this draws the general month layout
void month_drawer(int day, int month, int year,
    int date_storage[AMOUNT_DATES][3]) {
    static const char MONTHS[12][10] = {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December" };
    static const char DAYS[7][4] = { "Mon", "Tue", "Wed", "Thu",
        "Fri", "Sat", "Sun" };

    // this holds the days in the months which have an appointement so we can
    // mark them in the calender later on
    int date_days[AMOUNT_DATES] = { 0 };
    // this is the amount of appointments this month has
    int day_with_date = 0;

    // getting the first day of the week is important, this is where the first
    // number appears. Day counter is the actual number of the day printed, so
    // every print call effectively increases it by one
    int weekday = dayofweekcalculator(month, 1, year);
    int day_counter = 1;
    // this is used later to stop printing once the last day of the month is
    // reached
    int remaining_days = 0;

    // this iterates through the date storage. If the month + year fit, we add
    // the day to the array and increase the amount
    for (int n = 0; n < AMOUNT_DATES; n++) {
        if (date_storage[n][1] == month && date_storage[n][2] == year) {
            date_days[day_with_date] = date_storage[n][0];
            day_with_date++;
        }
    }
    
    // this prints the month and year, basically centered? month needs to be
    // one lower because its entered starting with 1, but the array 
    // starts with 0
    printf("%*s, %d\n", 30, MONTHS[month - 1], year);

    // this prints the name of the days
    for (int n = 0; n < 7; n++) {
        printf("%*s", 7, DAYS[n]);
    }
    printf("\n");

    // this is the first week of the month. It starts printing once n is equal
    // to weekday. E.g., monday is 0 and n is 0 on the first iteration.
    // Otherwise, we print 7 whitespaces, to keep the offset in the week
    for (int n = 0; n < 7; n++) {
        if (n >= weekday)
        {
            print_date(day, day_counter, day_with_date, date_days);
            day_counter++;
        }
        else
            printf("%*s", 7, "");
    }
    printf("\n");

    // this number is the amount of days the month has, minus however many days
    // we already printed in the first week
    remaining_days = DAY_MONTHS[month - 1] - (7 - weekday);

    // then this double for loop starts. A month can span maximal 6 weeks (see
    // the 11/2020 for example), the first week is already printed, thats why
    // n goes from 0 to 4. every week, the days are printed, as long as the
    // remaining days are larger then 0.
    for (int n = 0; n < 5; n++) {
        for (int i = 0; i < 7; i++) {
            if (remaining_days != 0) {
                print_date(day, day_counter, day_with_date, date_days);
                day_counter++;
                remaining_days--;
            }
            else
                break;
        }
        printf("\n");
    }
}

// this prints the correct number
void print_date(int day, int day_counter, int day_with_date,
    int date_days[AMOUNT_DATES]) {
    /*
    This function takes care of printing the specific day. A day can have four
    different representations. Just the number, the number with an * if there
    is an appointement at that day, nuimber + a x if that is the currently 
    selected day and the number + * + x if the day is selected and has an 
    appointement. 
    */
    // this is a check if a day with an appointment was already printed
    int match = 0; 

    // this for loop goes through the appointments for this month. If the day
    // is the one to print, it gets its *
    for (int i = 0; i < day_with_date; i++) {
        if (date_days[i] == day_counter) {
            match = 1;
            // if the day counter is also the selected day, the x is added
            if (day_counter == day)
                printf("%*dx*", 5, day_counter);
            else
                printf("%*d*", 6, day_counter);
            break;
        }
    }
    // if match is 0, there is no appointment at the day to print
    if (match == 0)
        if (day_counter == day)
            printf("%*dx", 6, day_counter);
        else
            printf("%*d", 7, day_counter);
}

/*The date/appointement functions */

// this allows to add an appointement to the storage
void add_date(int day, int month, int year, int date_storage[AMOUNT_DATES][3],
    char date_names[AMOUNT_DATES][50]) {
    char c; // used as (temporary) input storage
    char title[50]; // used as (temporary) longer input storage
    int validation = 0; // result of scanf_s or wrong user input
    int title_length = 0; // later lnegth of the entered title

    // that check gets increased for every set date. if it is as big as the 
    // AMOUNT_DATES constant, dates need to be deleted so none get overridden
    int check = 0;

    for (int n = 0; n < AMOUNT_DATES; n++) {
        // the delete function sets month to 0, so it doesn't get matched in the
        // month selector, and later here where the dates can be stored. So 
        // every non 0 value is an existing date.
        if (date_storage[n][1] != 0) {
            check++;
        }
    }
    // if the check is as big as the AMOUNT constant, no new ones can be stored, 
    // so here it's aborted
    if (check == AMOUNT_DATES) {
        printf("Sorry, you already maxed out the appointment storage in this \
application. Please go to one and delete it before commencing adding a new one.\
 Hit enter to continue.");
        scanf_s("%c", &c, 1);
        return;
    }

    // a new date can be added, asking if they are sure they want to add this 
    // specific date
    printf("You selected the following date to enter your appointment: \
%d.%d.%d. If you want to continue, hit y, otherwise n.", day, month, year);
    while (validation == 0) {
        validation = scanf_s(" %c", &c, 1);
        if (validation != 1 || (c != 'y' && c != 'n')) {
            printf("\nSorry, this was the wrong input. You can either hit 'n' \
to start again or 'y' to verify that this is the correct date.");
            validation = 0;
        }
    }
    if (c == 'n') {
        printf("\nAlright. Once you hit enter you will be warped back to the \
calendar. Pick the correct date and try again.");
        scanf_s("%c", &c, 1);
        return;
    }

    // resetting validation again since a new while loop starts 
    validation = 0;
    printf("\nGreat. We have the date, now we only need a name, which is no \
longer than 49 characters. Please only use alphanumerical latin characters \
excluding whitespacelike characters. Disclaimer: no special validation is \
applied. The code can not be hold responsible if you enter some weird chars \
and they can not be displayed properly.\n");
    while (validation == 0) {
        validation = scanf_s(" %s", &title, 49);
        if (validation != 1)
            printf("\nSorry, this was the wrong input. You need to enter a \
proper title.");
    }

    // if we don't add this here, the compiler complains about a potential non
    //  zero terminated string being passed to strlen. This is not the case, 
    // since scanf_s  always adds a \0 but what do I know Im just a student. 
    // Having more then one isn't a problem luckily, strlen just matches the 
    // first one.
    title[49] = '\0';

    // this for loop matches the first not set date and enters the date there
    for (int n = 0; n < AMOUNT_DATES; n++) {
        if (date_storage[n][1] == 0) {
            // this structure is not enforced, I need to remember this
            date_storage[n][0] = day;
            date_storage[n][1] = month;
            date_storage[n][2] = year;

            // adding one to the length ensures the \0 is copied as well
            title_length = strlen(title) + 1;
            for (int i = 0; i < title_length; i++) {
                // n is the same as in storage so its easy to know which titles belong
                // to which date
                date_names[n][i] = title[i];
            }
            break;
        }
    }
    printf("\nYour appointment is saved! Once you hit enter, you will be \
warped back to the calendar, and a * indicator will appear next to the day.");
    scanf_s("%c", &c, 1);
    return;
}

// this shows the appointments of a given date
void show_date(int day, int month, int year, int date_storage[AMOUNT_DATES][3],
    char date_titles[AMOUNT_DATES][50]) {
    // stores the indexes in date_titles for the titles of this day
    int title_indexes[AMOUNT_DATES] = { 0 };
    int index = 0; // amount of dates 
    char c; // non used input for scanf_s

    for (int n = 0; n < AMOUNT_DATES; n++) {
        // checks if a stored date matches the selected day
        if (date_storage[n][0] == day && date_storage[n][1] == month &&
            date_storage[n][2] == year) {
            // then stores the index (n) in the array
            title_indexes[index] = n;
            index++;
        }
    }
    // if index is 0, no date matches the selected day
    if (index == 0) {
        printf("Sorry, this day has no appointments. Hit enter to get to the\
 calendar and select a correct date.");
        scanf_s("%c", &c, 1);
        return;
    }

    printf("The following appointments are from this date:\n");
    // now we loop through the known indexes and print out the stored string
    for (int n = 0; n < index; n++) {
        printf("* %s\n", date_titles[title_indexes[n]]);
    }
    printf("\nOnce you hit enter, you will be warped back to the calendar.");
    scanf_s("%c", &c, 1);
    return;
}

// this deletes the appointments at a specific date
void delete_date(int day, int month, int year,
    int date_storage[AMOUNT_DATES][3], char date_titles[AMOUNT_DATES][50]) {
    int date_indexes[AMOUNT_DATES] = { 0 }; // array of wanted indexes
    int index = 0; // amount of indexes
    char c; // char to store the single character input of the user
    int validation = 0; // return of scanf_s, or wrong input from the user

    // same loop as in show_date
    for (int n = 0; n < AMOUNT_DATES; n++) {
        if (date_storage[n][0] == day && date_storage[n][1] == month 
            && date_storage[n][2] == year) {
            date_indexes[index] = n;
            index++;
        }
    }
    // also the same as in show_date
    if (index == 0) {
        printf("Sorry, this day has no appointments you could delete. Hit \
enter to get to the calendar and select a correct date.");
        scanf_s("%c", &c, 1);
        return;
    }
    // again, the same as in show_date
    printf("The following appointments are from this date:\n");
    for (int n = 0; n < index; n++) {
        printf("* %s\n", date_titles[date_indexes[n]]);
    }
    printf("If you want to delete them, hit y, otherwise n.");
    // simple no yes input, same as in add_date
    while (validation == 0) {
        validation = scanf_s(" %c", &c, 1);
        if (validation != 1 || (c != 'y' && c != 'n')) {
            printf("\nSorry, this was the wrong input. You can either hit 'n' \
to stop the deletion or 'y' to go forward with it.");
            validation = 0;
        }
    }
    if (c == 'n') {
        printf("\nNo deletions were made. Once you hit enter you will be \
warped back to the calendar.");
        scanf_s("%c", &c, 1);
        return;
    }

    // setting [n][1] to 0 is enough since that value is checked by the month
    // drawer and the add_date function
    for (int n = 0; n < index; n++) {
        date_storage[date_indexes[n]][1] = 0;
    }
    printf("\nYour appointment(s) were deleted! Once you hit enter, you will be warped back to the calendar.");
    scanf_s("%c", &c, 1);
    return;
}