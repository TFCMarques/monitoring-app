#include <windows.h>
#include <stdio.h>

#include "communication.h"
#include "parsers.h"
#include "requests.h"
#include "utils.h"
 
int main() {
    HANDLE hSerial;

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

            if(received == NULL || received[0] == '\0') continue;
            //printf("Received = %s\n\n", received);

            int isWarning = checkIfWarning(received);
            
            if (isWarning == 0) {
                int* values = parseMessage(received, 3);

                values[0] = values[0] / 7;
                values[1] = values[1] * 100 / 1023;
                values[2] = 5 * values[2] * 100 / 1023;

                printf("> Message received: \n");
                printf("- Wind: %d RPM\n", values[0]);
                printf("- Humidity: %d%%\n", values[1]);
                printf("- Temperature: %d Celsius\n", values[2]);

                printf("\n> Saving message in XML file.\n");
                writeMsgToXML(values, currentTime); 
                free(values);
            } else {
                char* warning;
                int* values = parseMessage(received, 4);

                printf("> Warning received: \n");
                switch(values[0]) {
                    case 1: warning = "Wind Gusts"; break;
                    case 2: warning = "Heavy Rainfall"; break;
                    case 3: warning = "Wildfire Hazard"; break;
                    // ...
                    default: warning = "N/A";
                } 

                values[1] = values[1] / 7;
                values[2] = values[2] * 100 / 1023;
                values[3] = 5 * values[3] * 100 / 1023;

                printf("- Type: %s\n", warning);
                printf("- Wind: %d RPM\n", values[1]);
                printf("- Humidity: %d%%\n", values[2]);
                printf("- Temperature: %d Celsius\n", values[3]);

                printf("\n> Saving warning in XML file.\n");
                writeWarningToXML(warning, values, currentTime);
                free(values);
            }

            // Post received JSON to server before freeing up memory
            
            initWinsock();
            SOCKET sck = createSocket();
            if (sck == 0 || !connectToServer(sck)) return 1;
            postData(sck, received);
            closeSocket(sck);

            free(currentTime);
            free(received);
        }

        closeSerialPort(hSerial);
    }

    return 0;
}