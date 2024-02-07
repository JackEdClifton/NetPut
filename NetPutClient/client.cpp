
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>

#include "../Common/common.h"

bool register_window(HINSTANCE, HINSTANCE, LPWSTR, int);
void handle_input(SOCKET);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	// setup winsock
	SOCKET sock = 0;
	if (setup_winsock(sock)) {
		return 1;
	}

	// connect socket
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, L"192.168.0.21", &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(sock, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		std::cout << "Client: connect() failed: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	
	std::thread t1 = std::thread(handle_input, sock);

	// Message loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	

	return 0;
}