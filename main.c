#include <windows.h>
#include <stdio.h>
#include "communication.h"
 
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
            free(received);
        }

        closeSerialPort(hSerial);
    }

    return 0;
}