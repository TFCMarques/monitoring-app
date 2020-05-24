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
    } else fprintf(stderr, "OK.\n\n");

    return hSerial;
}

int setDCBParameters(HANDLE hSerial) {
    // Initializing DCB struct
    DCB dcbSerialParams = {0};

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return 0;
    }

    // Set device parameters (9200 baud, 1 start bit, 1 stop bit, no parity)
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return 0;
    } else return 1;
}

int setTimeouts(HANDLE hSerial) {
    COMMTIMEOUTS timeouts = {0};

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 0;
    } else return 1;
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
		fprintf(stderr, "%ld bytes written\n", bytesWritten);
		return 1;
	}
}

char* receiveData(HANDLE hSerial) {
	int i = 0, j;
    DWORD readBytes;
    char* dataBuffer = (char*) malloc(sizeof(char) * 512);
    char currentChar;

    do {
        if(!ReadFile(hSerial, &currentChar, sizeof(currentChar), &readBytes, NULL)) {
            fprintf(stderr, "Error receiving bytes.\n");
            CloseHandle(hSerial);
       		return NULL;
		} else {
			dataBuffer[i++] = currentChar;
		}
    } while(readBytes > 0);

	// Remove last 2 duplicated chars
	char* response = (char*) malloc(sizeof(char) * 512);
	for(j = 0; j < i - 2; j++) {
		response[j] = dataBuffer[j];
	}
	
	free(dataBuffer);
	response[j] = '\0';
	return response;
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