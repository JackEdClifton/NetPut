#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>

#include "../Common/common.h"


void GetWindowPos(HWND hwnd, int* x, int* y) {
	RECT rect = { NULL };
	if (GetWindowRect(hwnd, &rect)) {
		*x = rect.left;
		*y = rect.top;
	}
	int __uwu = 5;
}

bool IsSocketConnected(SOCKET socket) {
	fd_set readSet;
	fd_set writeSet;
	fd_set exceptSet;
	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	FD_ZERO(&exceptSet);
	FD_SET(socket, &readSet);
	FD_SET(socket, &writeSet);
	FD_SET(socket, &exceptSet);

	timeval timeout = { 0, 0 };  // Zero timeout for a quick check

	int result = select(0, &readSet, &writeSet, &exceptSet, &timeout);

	if (result == SOCKET_ERROR) {
		// Handle error (e.g., call WSAGetLastError())
		return false;
	}

	return (result > 0 && (FD_ISSET(socket, &readSet) || FD_ISSET(socket, &writeSet) || FD_ISSET(socket, &exceptSet)));
}

int connect_socket(SOCKET& sock) {
	// connect socket
	static sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, L"192.168.0.188", &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(sock, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		std::cout << "Client: connect() failed: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		return 1;
	}
	return 0;
}


void handle_input(bool* quit, HWND* hwnd, SOCKET sock) {
	// chat to serv
	static char buffer[200];
	memset(buffer, 0, sizeof(buffer));

	static int x, y;
	static bool run = false;
	static int byteCount;
	static long long last_f7_press = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	static long long this_f7_press;

	// calling GetAsyncKeyState with the
	// mouse events causes bouncing
	// if we increment this each iteration,
	// and only call GetAsync on the overflow
	// we can keep mouse positions responsive
	// without the mouse buttons being bad
	char get_async_delay = -1;

input:


	if (!IsSocketConnected(sock)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		goto input;
	}

	if (*quit) {
		return;
	}

	if (run) {
		// get mouse position data
		GetWindowPos(*hwnd, &x, &y);
		GetCursorPos(&events.type.POINT);
		x += 100; y += 100;
		SetCursorPos(x, y);
		events.type.POINT.x -= x;
		events.type.POINT.y -= y;

		// get mouse button data
		get_async_delay++;
		if (get_async_delay == 60) {
			get_async_delay = 0;
		}
		
		events.type.L_MOUSE_BTN = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		events.type.R_MOUSE_BTN = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
		events.type.M_MOUSE_BTN = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	}
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}


	// send inputs to server
	byteCount = send(sock, events._buffer, EVENT_BUFF_SIZE, 0);
	events.type.MOUSE_WHEEL = 0; // reset this value

	if (byteCount <= 0) {
		return; // exit program
	}

	if (GetAsyncKeyState(VK_F7)) {
		this_f7_press = std::chrono::high_resolution_clock::now().time_since_epoch().count();

#define _2seconds 2000000000
		if (last_f7_press + _2seconds < this_f7_press) {
			run = !run;
			last_f7_press = this_f7_press;
		}
	}

	// mouse click events
	if (0) {

	}


	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	goto input;
}