#include <windows.h>
#include <stdio.h>
#include "communication.h"
 
int main() {
    HANDLE hSerial; 
    char* received;
 
    // Opening COM1 to send data
    hSerial = openSerialPort("\\\\.\\COM1");

    if (hSerial != NULL) {
        setDCBParameters(hSerial);
        setTimeouts(hSerial);
        setReceivingMask(hSerial);

        setWaitCommEvent(hSerial);
        received = receiveData(hSerial);
        printf("\nString received: %s\n", received);
    }

    closeSerialPort(hSerial);
    return 0;
}