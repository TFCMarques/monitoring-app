#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* getDatetime() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char* timeString = (char*) malloc(sizeof(char) * 18);

    sprintf(timeString, "%02d/%02d/%d - %02d:%02d",
            tm.tm_mday,  tm.tm_mon + 1, (tm.tm_year + 1900), tm.tm_hour, tm.tm_min);
        
    return timeString;
}