#include<stdio.h>
#include<winsock2.h>
#include "requests.h"

int initWinsock() {
    WSADATA wsa;

    printf("> Initializing socket: ");
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error.\n");
        return 0;
    } else printf("OK.\n");
    
    return 1;
}

SOCKET createSocket() {
    printf("> Creating socket: ");
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == INVALID_SOCKET) {
        printf("Error.\n");
        return 0;
    } else printf("OK.\n");

    return sock;
}

int connectToServer(SOCKET socket) {
    struct sockaddr_in server;

    printf("> Connecting to server: ");
    server.sin_addr.S_un.S_addr = inet_addr("193.136.120.133");
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    int isConnected = connect(socket, (struct sockaddr *) &server, sizeof(server));

    if (isConnected < 0) {
        printf("Error.\n");
        return 0;
    } else  printf("OK.\n");

    return 1;
}

int postData(SOCKET socket, char* data) {
    char raw[512];
    char contentLength[32];
    char* prefix = "POST /~sad/ HTTP/1.1\r\n";
    char* host = "Host: 193.136.120.133\r\n";
    char* contentType = "Content-Type: application/json\r\n";

    snprintf(contentLength, sizeof(contentLength), "Content-Length: %d\r\n\r\n", strlen(data));
    snprintf(raw, sizeof(raw), "%s%s%s%s%s", prefix, host, contentType, contentLength, data);

    printf("> Posting to server: ");
    int sentData = send(socket, raw, strlen(raw), 0);

	if(sentData < 0) {
        printf("Error %d.\n", WSAGetLastError());
        return 0;
    } else printf("OK.\n");

	return 1;
}

void closeSocket(SOCKET socket) {
    printf("> Closing socket: ");
    if (closesocket(socket) == 0) {
        WSACleanup();
        printf("OK.\n\n");
    }
}