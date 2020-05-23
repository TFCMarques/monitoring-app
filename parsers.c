#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* parseMessage(char* jsonString, int start, int numValues) {
    int j = 0;
    int digit = 0;
    int quotes = 0;
    int length = strlen(jsonString);
    int* parsedValues = (int*) malloc(sizeof(int) * numValues);

    for(int i = 0; i < length; i++) {
        if (jsonString[i] == '\"') {
            quotes++;
        } else if(quotes >= start && quotes % 2 == 0) {
            int number = 0;

            while(jsonString[i] >= '0' && jsonString[i] <= '9') {
                digit = jsonString[i] - '0';
                number = number * 10 + digit;
                i++;
            }

            if(number != 0) {
                parsedValues[j] = number;
                j++;
            }
        }
    }

    return parsedValues;
}

void writeMsgToXML(int* values) {

}

void writeWarningToXML(int* values) {

}