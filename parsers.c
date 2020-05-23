#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void writeMsgToXML(int* values, char* datetime) {
    FILE *fileXML;

    fileXML = fopen("logger.xml", "ab+");

    fprintf(fileXML, "<msg1>\n");
    fprintf(fileXML, "\t<datetime> %s </datetime>\n", datetime);
    fprintf(fileXML, "\t<wind> %d </wind>\n", values[0]);
    fprintf(fileXML, "\t<humidity> %d </humidity>\n", values[1]);
    fprintf(fileXML, "\t<temperature> %d </temperature>\n", values[2]);
    fprintf(fileXML, "</msg1>\n");

    fclose(fileXML);
}

void writeWarningToXML(int* values, char* datetime) {
    FILE *fileXML;

    fileXML = fopen("logger.xml", "ab+");

    fprintf(fileXML, "<warning>\n");
    fprintf(fileXML, "\t<datetime> %s </datetime>\n", datetime);
    fprintf(fileXML, "\t<type> %d </type>\n", values[0]);
    fprintf(fileXML, "\t<wind> %d </wind>\n", values[1]);
    fprintf(fileXML, "\t<humidity> %d </humidity>\n", values[2]);
    fprintf(fileXML, "\t<temperature> %d </temperature>\n", values[3]);
    fprintf(fileXML, "</warning>\n");

    fclose(fileXML);
}