
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#include "../Common/common.h"


int main() {

	// setup winsock
	SOCKET sock;
	if (setup_winsock(&sock)) {
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

	// setup listen
	if (listen(sock, 1) == SOCKET_ERROR) {
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return 1;
	}

listen:
	std::cout << "Info: Listening for connection\n";

	// accept connection
	SOCKET acceptSocket = accept(sock, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	std::cout << "Accepted connection" << std::endl;

	// chat to client
	POINT delta_p;
	short l_btn = 0;
	short r_btn = 0;
	short l_btn_in = 0;
	short r_btn_in = 0;

input:
	int byteCount = recv(acceptSocket, events._buffer, EVENT_BUFF_SIZE, 0);

	if (byteCount <= 0) {
		goto listen;
	}

	if (events.type.POINT.x || events.type.POINT.y) {
		std::cout << "Mouse Pos: " << events.type.POINT.x << ", " << events.type.POINT.y << std::endl;
	}

	// mouse mouse
	GetCursorPos(&delta_p);
	delta_p.x += events.type.POINT.x;
	delta_p.y += events.type.POINT.y;
	SetCursorPos(delta_p.x, delta_p.y);
	delta_p.x = 0;
	delta_p.y = 0;

	// get mouse button data
	memcpy(&l_btn_in, &events.type.L_MOUSE_BTN, 2);
	memcpy(&r_btn_in, &events.type.R_MOUSE_BTN, 2);

	// left clicks
	if (l_btn_in != l_btn) {
		if (l_btn_in) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		}
		else {
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		l_btn = l_btn_in;
	}

	// right clicks
	if (r_btn_in != r_btn) {
		if (r_btn_in) {
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		}
		else {
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
		}
		r_btn = r_btn_in;
	}

	// middle mouse btn
	if (events.type.MOUSE_WHEEL) {
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, events.type.MOUSE_WHEEL, 0);
	}

	goto input;

	WSACleanup();
	return 0;
}


/*

TODO:

create window

intercept mouse clicks
or put the mouse on a window and forward clicks (prefered)

then do the same with keyboard events

will it work on UAC?

*/