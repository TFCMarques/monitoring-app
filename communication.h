#include <windows.h>

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

HANDLE initCommunication();
void sendData(HANDLE hSerial, char* data);
void closeSerialPort(HANDLE hSerial);

#endif	/* COMMUNICATION_H */