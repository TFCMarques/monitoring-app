#include <windows.h>
#include <stdio.h>

#include "communication.h"
#include "parsers.h"
 
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
            char* received = receiveData(hSerial);
            printf("\nString received: %s\n", received);
            int* values = parsePeriodicMsg(received, 2, 3);
            printf("Values: %d, %d, %d\n", values[0], values[1], values[2]);
            free(received);
            free(values);
        }

        closeSerialPort(hSerial);
    }

    return 0;
}