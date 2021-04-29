// C program to print the month by month
// calendar for the given year

#include <xinu.h>
// Function that returns the index of the
// day for date DD/MM/YYYY
int dayNumber(int day, int month, int year)
{

    static int t[] = {0, 3, 2, 5, 0, 3,
                      5, 1, 4, 6, 2, 4};
    year -= month < 3;
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

// Function that returns the name of the
// month for the given month Number
// January - 0, February - 1 and so on
char *getMonthName(int monthNumber)
{
    char *month;

    switch (monthNumber)
    {
    case 0:
        month = "January";
        break;
    case 1:
        month = "February";
        break;
    case 2:
        month = "March";
        break;
    case 3:
        month = "April";
        break;
    case 4:
        month = "May";
        break;
    case 5:
        month = "June";
        break;
    case 6:
        month = "July";
        break;
    case 7:
        month = "August";
        break;
    case 8:
        month = "September";
        break;
    case 9:
        month = "October";
        break;
    case 10:
        month = "November";
        break;
    case 11:
        month = "December";
        break;
    }
    return month;
}

// Function to return the number of days
// in a month
int numberOfDays(int monthNumber, int year)
{
    // January
    if (monthNumber == 0)
        return (31);

    // February
    if (monthNumber == 1)
    {
        // If the year is leap then Feb
        // has 29 days
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
            return (29);
        else
            return (28);
    }

    // March
    if (monthNumber == 2)
        return (31);

    // April
    if (monthNumber == 3)
        return (30);

    // May
    if (monthNumber == 4)
        return (31);

    // June
    if (monthNumber == 5)
        return (30);

    // July
    if (monthNumber == 6)
        return (31);

    // August
    if (monthNumber == 7)
        return (31);

    // September
    if (monthNumber == 8)
        return (30);

    // October
    if (monthNumber == 9)
        return (31);

    // November
    if (monthNumber == 10)
        return (30);

    // December
    if (monthNumber == 11)
        return (31);
    return 0;
}

// Function to print the calendar of
// the given year
void printCalendar(int year, int month, int date)
{
    printf("	 Calendar - %d\n\n", year);
    int days;

    // Index of the day from 0 to 6
    int current = dayNumber(1, 1, year);

    // i for Iterate through months
    // j for Iterate through days
    // of the month - i
    int i;
    for (i = 0; i < 12; i++)
    {
        // skip other months
        if (month - 1 != i) 
        {
            continue;
        }

        days = numberOfDays(i, year);

        // Print the current month name
        printf("\n");
        printf("--------------------");
        printf("\033[4;35;47m%s\033[0m", getMonthName(i));
        printf("--------------------\n\n");
        // 		printf("\n--------------------%s--------------------\n\n",
        // 			getMonthName(i));

        char names[7][5] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

        // Print the columns
        int d;
        for (d = 0; d < 7; d++)
        {
            printf("%s\t", names[d]);
        }
        printf("\n");

        // Print appropriate spaces
        int k;
        for (k = 0; k < current; k++)
            printf(" \t");
        int j;
        for (j = 1; j <= days; j++)
        {
            if (j == date && i == (month - 1))
            {
                //textbackground(RED);
                printf("\033[4;31;47m%d\033[0m", j);
                printf("\t");
            }
            else
            {
                printf("%d\t", j);
            }
            if (++k > 6)
            {
                k = 0;
                printf("\n");
            }
        }

        if (k)
            printf("\n");

        current = k;
    }

    return;
}