#include <windows.h>

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

HANDLE openSerialPort();
int setDCBParameters(HANDLE hSerial);
int setTimeouts(HANDLE hSerial);
int setReceivingMask(HANDLE hSerial);
int setWaitCommEvent(HANDLE hSerial);
int sendData(HANDLE hSerial, char* data);
char* receiveData(HANDLE hSerial);
int closeSerialPort(HANDLE hSerial);

#endif	/* COMMUNICATION_H */