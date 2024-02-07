#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>

union _point {
	POINT POINT;
	char buffer[8];
};

void GetWindowPos(int* x, int* y) {
	RECT rect = { NULL };
	if (GetWindowRect(GetConsoleWindow(), &rect)) {
		*x = rect.left;
		*y = rect.top;
	}
}


void handle_input(SOCKET clientSocket) {
	// chat to serv
	static char buffer[200];
	memset(buffer, 0, sizeof(buffer));

	static _point p;
	static int x, y;
	static bool run = false;
	static int byteCount;
	static long long last_f7_press = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	static long long this_f7_press;

input:

	if (run) {
		// get cursor reset target pos
		GetWindowPos(&x, &y);
		GetCursorPos(&p.POINT);
		x += 100; y += 100;
		SetCursorPos(x, y);
		p.POINT.x -= x;
		p.POINT.y -= y;
	}
	else {
		p.POINT.x = 0;
		p.POINT.y = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	byteCount = send(clientSocket, p.buffer, 8, 0);

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


	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	goto input;
}