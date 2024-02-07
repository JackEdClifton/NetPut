
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#include "../Common/common.h"

union _point {
	POINT POINT;
	char buffer[8];
};

int main() {

	// setup winsock
	SOCKET sock = 0;
	if (setup_winsock(sock)) {
		return 1;
	}

	// bind socket
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, L"0.0.0.0", &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(sock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return 0;
	}

	else {
		std::cout << "bind is ok" << std::endl;
	}

	// setup listen
	if (listen(sock, 1) == SOCKET_ERROR) {
		std::cout << "Listen(): Error listening on socket " << WSAGetLastError() << std::endl;
	}

	// accept connection
	SOCKET acceptSocket = accept(sock, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "Accept failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	std::cout << "Accepted connection" << std::endl;

	// chat to client
	_point p;
	POINT delta_p;

input:
	int byteCount = recv(acceptSocket, p.buffer, 8, 0);

	if (byteCount <= 0) {
		goto exit;
	}

	if (p.POINT.x || p.POINT.y) {
		std::cout << "Mouse Pos: " << p.POINT.x << ", " << p.POINT.y << std::endl;
	}

	// mouse mouse
	GetCursorPos(&delta_p);
	delta_p.x += p.POINT.x;
	delta_p.y += p.POINT.y;
	//SetCursorPos(delta_p.x, delta_p.y);
	delta_p.x = 0;
	delta_p.y = 0;

	goto input;

exit:
	WSACleanup();
}


/*

TODO:

create window

intercept mouse clicks
or put the mouse on a window and forward clicks (prefered)

then do the same with keyboard events

will it work on UAC?

*/