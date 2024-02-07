
#include <iostream>
#include <winsock2.h>

#include "common.h"

int port = 55555;


bool setup_winsock(SOCKET sock) {

	// setup DLL
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr) {
		std::cout << "Error: Winsock dll not found\n";
		return WINSOCK_DLL_ERROR;
	}

	// setup socket
	sock = INVALID_SOCKET;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		std::cout << "Error: Could not setup socket" << WSAGetLastError() << std::endl;
		WSACleanup();
		return SOCK_SETUP_ERROR;
	}

	return 0;
}