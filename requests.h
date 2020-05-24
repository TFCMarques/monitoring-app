#include <winsock2.h>

#ifndef REQUESTS_H
#define	REQUESTS_H

int initWinsock();
SOCKET createSocket();
int connectToServer(SOCKET socket);
int postData(SOCKET socket, char* data);
void closeSocket(SOCKET socket);

#endif	/* REQUESTS_H */