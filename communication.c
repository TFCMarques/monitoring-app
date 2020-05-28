#include <windows.h>
#include <stdio.h>
#include <string.h>

HANDLE openSerialPort(char * serialPort) {
    HANDLE hSerial;

    fprintf(stderr, "> Opening serial port: ");
    hSerial = CreateFile(serialPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error.\n");
        return NULL;
    } else fprintf(stderr, "OK.\n");

    // Initializing DCB struct
    DCB dcbSerialParams = {0};

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    fprintf(stderr, "> Setting DCB Parameters: ");
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting device state.\n");
        CloseHandle(hSerial);
        return NULL;
    }

    // Set device parameters (9200 baud, 1 start bit, 1 stop bit, no parity)
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting device parameters.\n");
        CloseHandle(hSerial);
        return NULL;
    } else fprintf(stderr, "OK.\n");

    COMMTIMEOUTS timeouts = {0};

    fprintf(stderr, "> Setting COM timeouts: ");
    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 1000;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        fprintf(stderr, "Error setting timeouts.\n");
        CloseHandle(hSerial);
        return NULL;
    } else fprintf(stderr, "OK.\n\n");

    return hSerial;
}

int setReceivingMask(HANDLE hSerial) {
    if (!SetCommMask(hSerial, EV_RXCHAR)) {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 0;
    } else return 1;
}

int setWaitCommEvent(HANDLE hSerial) {
    DWORD dwEventMask;

    if (!WaitCommEvent(hSerial, &dwEventMask, NULL)) {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 0;
    } else return 1; 
}

int sendData(HANDLE hSerial, char* data) {
    DWORD bytesWritten;

    fprintf(stderr, "Sending bytes: ");

    if (!WriteFile(hSerial, data, strlen(data), &bytesWritten, NULL)) {
        fprintf(stderr, "Error\n");
        CloseHandle(hSerial);
		return 0;
    } else {
		fprintf(stderr, "%ld bytes written\n\n", bytesWritten);
		return 1;
	}
}

char* receiveData(HANDLE hSerial) {
	int i = 0;
    DWORD readBytes;
    char* dataBuffer = (char*) malloc(sizeof(char) * 512);
    char currentChar;

    do {
        ReadFile(hSerial, &currentChar, sizeof(currentChar), &readBytes, NULL);

        if(i == 0 && currentChar != '{') return NULL;

        if(readBytes > 0 && currentChar != '\r' && currentChar != '\n') {
            dataBuffer[i++] = currentChar;
        }
    } while(currentChar != '\n');

    dataBuffer[i] = '\0';
	return dataBuffer;
}

int closeSerialPort(HANDLE hSerial) {
    fprintf(stderr, "Closing serial port: ");

    if (!CloseHandle(hSerial)) {
        fprintf(stderr, "Error\n");
		return 0;
    } else {
		fprintf(stderr, "OK\n");
		return 1;
	}
}