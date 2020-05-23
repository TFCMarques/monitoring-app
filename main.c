#include <windows.h>
#include <stdio.h>

#include "communication.h"
#include "parsers.h"
#include "utils.h"
 
int main() {
    HANDLE hSerial; ;
 
    // Opening COM1 to send data
    hSerial = openSerialPort("\\\\.\\COM2");

    if (hSerial != NULL) {
        setDCBParameters(hSerial);
        setTimeouts(hSerial);
        setReceivingMask(hSerial);

        while(1) {
            setWaitCommEvent(hSerial);
            char* currentTime = getDatetime();
            char* received = receiveData(hSerial);

            int isWarning = checkIfWarning(received);
            
            if (!isWarning) {
                int* values = parseMessage(received, 3);

                values[0] = values[0] / 7;
                values[1] = values[1] * 100 / 1023;
                values[2] = 5 * values[2] * 100 / 1023;

                printf("> Message received: \n");
                printf("- Wind: %d RPM\n", values[0]);
                printf("- Humidity: %d%%\n", values[1]);
                printf("- Temperature: %d Celsius\n", values[2]);

                printf("\n> Saving message in XML file.\n\n");
                writeMsgToXML(values, currentTime); 
                free(values);
            } else {
                int* values = parseMessage(received, 4);

                printf("> Warning received: \n");
                switch(values[0]) {
                    case 1: printf("- Type: Tornado\n"); break;
                    case 2: printf("- Type: Heavy Rainfall\n"); break;
                    case 3: printf("- Type: Wildfire Hazard\n"); break;
                    // ...
                    default: printf("- Type: N/A\n");
                } 

                values[1] = values[1] / 7;
                values[2] = values[2] * 100 / 1023;
                values[3] = 5 * values[3] * 100 / 1023;

                printf("- Wind: %d RPM\n", values[1]);
                printf("- Humidity: %d%%\n", values[2]);
                printf("- Temperature: %d Celsius\n", values[3]);

                printf("\n> Saving warning in XML file.\n\n");
                writeWarningToXML(values, currentTime); 
                free(values);
            }

            free(currentTime);
            free(received);
        }

        closeSerialPort(hSerial);
    }

    return 0;
}