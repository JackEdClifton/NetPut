
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>
#include <fstream>

#include "../Common/common.h"

bool register_window(HWND*, HINSTANCE, HINSTANCE, LPWSTR, int);
void handle_input(bool*, HWND*, SOCKET);

SOCKET sock;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	HWND hwnd;
	register_window(&hwnd, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// setup winsock
	if (setup_winsock(&sock)) {
		return 1;
	}
		
	bool stop_t1 = false;
	std::thread t1 = std::thread(handle_input, &stop_t1, &hwnd, sock);

	// Message loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	stop_t1 = true;
	t1.join();

	closesocket(sock);
	WSACleanup();

	return 0;
}