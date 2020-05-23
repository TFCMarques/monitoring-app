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
            int* values = parseMessage(received, 2, 3);

            writeMsgToXML(values, currentTime);

            free(currentTime);
            free(received);
            free(values);
        }

        closeSerialPort(hSerial);
    }

    return 0;
}