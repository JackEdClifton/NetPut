#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>

#include "../Common/common.h"


void GetWindowPos(HWND hwnd, int* x, int* y) {
	RECT rect = { NULL };
	if (GetWindowRect(hwnd, &rect)) {
		*x = rect.left;
		*y = rect.top;
	}
	int __uwu = 5;
}

void get_ip_list() {
	std::string filename = "ip.txt";
	std::ifstream inputFile(filename);

	if (!inputFile.is_open()) {
		// create file with dummy IP
	}
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

	// read ip from file
	std::string ip;
	{
		std::ifstream ipFile("ip.txt");
		std::getline(ipFile, ip);
		ipFile.close();
	}
	
	// write to file if it doesnt exist
	if (ip.length() == 0)
	{
		ip = "172.0.0.1";
		std::ofstream ipFile("ip.txt");
		ipFile << ip;
		ipFile.close();
	}

	// connect socket
	static sockaddr_in clientService;
	clientService.sin_family = AF_INET;

	// loop over IPs until valid connection is made
	inet_pton(AF_INET, ip.c_str(), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(get_port());
	if (connect(sock, (SOCKADDR*)&clientService, sizeof(clientService)) != SOCKET_ERROR) {
		return 0;
	}

	// none of the listed IPs could provide a connection
	closesocket(sock);
	return 1;
}


void handle_input(bool* quit, HWND* hwnd, SOCKET sock) {
	static int x, y;
	static bool run = false;
	static int byteCount;
	static long long last_f7_press = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	static long long this_f7_press;


check_connection_loop:

	if (!IsSocketConnected(sock)) {
		SetDlgItemText(*hwnd, 50, L"Disconnected");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		goto check_connection_loop;
	}

	else {
		SetDlgItemText(*hwnd, 50, L"Connected");

	}


input:


	if (!IsSocketConnected(sock)) {
		goto check_connection_loop;
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

		events.type.L_MOUSE_BTN = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		events.type.R_MOUSE_BTN = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
		events.type.M_MOUSE_BTN = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	}
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	// send inputs to server
	byteCount = send(sock, events._buffer, EVENT_BUFF_SIZE, 0);
	event_buff_reset();

	if (byteCount <= 0) {
		return; // exit program
	}

	if (GetAsyncKeyState(VK_F7)) {
		this_f7_press = std::chrono::high_resolution_clock::now().time_since_epoch().count();

#define delay_ms 1000000 * 200
		if (last_f7_press + delay_ms < this_f7_press) {
			run = !run;
			last_f7_press = this_f7_press;

			// change UI name
			const wchar_t* help_texts[] = {
				L"Disabled",
				L"Enabled"
			};

			SetDlgItemText(*hwnd, 50, help_texts[run]);
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	goto input;
}


/*
 * TODO:
 *
 * Stop the mouse click bounce (again)
 *
 * Get IP address(es) from file
 * Create file with dummy IP if does not exist
 *
 * Make UI better, colour code
 * grey		- disconnected
 * green	- connected
 * yellow	- active
 *
 * Intercept Alt and Win buttons
 */