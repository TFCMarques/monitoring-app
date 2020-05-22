#include <windows.h>
#include <stdio.h>
#include "communication.h"
 
int main()
{
    char hello[] = "Hello World!\n";
 
    HANDLE hSerial = initCommunication();
    sendData(hSerial, hello);

    receiveData(hSerial);

    closeSerialPort(hSerial);

    return 0;
}