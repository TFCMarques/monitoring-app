#include <windows.h>
#include <stdio.h>
#include <string.h>

HANDLE initCommunication() {
    // Declare variables and structures
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // Open the highest available serial port number
    fprintf(stderr, "Opening serial port: ");
    hSerial = CreateFile("\\\\.\\COM2", GENERIC_READ | GENERIC_WRITE, 0, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error\n");
        return NULL;
    } else fprintf(stderr, "OK\n");

    // Set device parameters (9200 baud, 1 start bit, 1 stop bit, no parity)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0) {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return NULL;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (SetCommState(hSerial, &dcbSerialParams) == 0) {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return NULL;
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (SetCommTimeouts(hSerial, &timeouts) == 0) {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return NULL;
    }

    return hSerial;
}

void sendData(HANDLE hSerial, char* data) {
    DWORD bytes_written;

    fprintf(stderr, "Sending bytes: ");

    if (!WriteFile(hSerial, data, strlen(data), &bytes_written, NULL)) {
        fprintf(stderr, "Error\n");
        CloseHandle(hSerial);
    } else fprintf(stderr, "%d bytes written\n", bytes_written);
}

void receiveData(HANDLE hSerial) {
    DWORD readBytes;
    char dataBuffer[100];
    int state = 1;
    char currentChar;

    fprintf(stderr, "Receiving bytes: ");

    if(!ReadFile(hSerial, &currentChar, 1, &readBytes, NULL)) {
        fprintf(stderr, "Error\n");
        CloseHandle(hSerial);
        return;
    } else {
        fprintf(stderr, "OK\n");

        while(1) {
            strncat(dataBuffer, &currentChar, 1);

            if (currentChar == '\n') {
                fprintf(stdout, "%s\n", dataBuffer);
                return;
            }

            if(!ReadFile(hSerial, &currentChar, 1, &readBytes, NULL)) {
                fprintf(stderr, "Receiving bytes: Error\n");
                CloseHandle(hSerial);
                return;
            }
        }
    }
}

void closeSerialPort(HANDLE hSerial) {
    fprintf(stderr, "Closing serial port: ");

    if (CloseHandle(hSerial) == 0) {
        fprintf(stderr, "Error\n");
    } else fprintf(stderr, "OK\n");
}