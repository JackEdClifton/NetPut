
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#include "../Common/common.h"


int main() {

	// setup winsock
	SOCKET listen_sock;
	if (setup_winsock(&listen_sock)) {
		return 1;
	}

	// bind socket
	sockaddr_in service;
	int port = get_port();
	service.sin_family = AF_INET;
	InetPton(AF_INET, L"0.0.0.0", &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(listen_sock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind failed" << WSAGetLastError() << std::endl;
		closesocket(listen_sock);
		WSACleanup();
		return 0;
	}

	// setup listen
	if (listen(listen_sock, 1) == SOCKET_ERROR) {
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return 1;
	}

listen:
	std::cout << "Info: Listening for connection on port " << port << "\n";

	// accept connection
	SOCKET sock = accept(listen_sock, NULL, NULL);
	if (sock == INVALID_SOCKET) {
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}
	
	std::cout << "Accepted connection\n";

	// chat to client
	POINT delta_p;
	unsigned short l_btn = 0;
	unsigned short r_btn = 0;
	unsigned short m_btn = 0;

input:
	int byteCount = recv(sock, events._buffer, EVENT_BUFF_SIZE, 0);

	if (byteCount <= 0) {
		std::cout << "Connection Lost\n";
		goto listen;
	}

	// mouse mouse
	GetCursorPos(&delta_p);
	delta_p.x += events.type.POINT.x;
	delta_p.y += events.type.POINT.y;
	SetCursorPos(delta_p.x, delta_p.y);
	delta_p.x = 0;
	delta_p.y = 0;

	// left clicks
	if (events.type.L_MOUSE_BTN != l_btn) {
		if (events.type.L_MOUSE_BTN) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		}
		else {
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		l_btn = events.type.L_MOUSE_BTN;
	}

	// right clicks
	if (events.type.R_MOUSE_BTN != r_btn) {
		if (events.type.R_MOUSE_BTN) {
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		}
		else {
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
		}
		r_btn = events.type.R_MOUSE_BTN;
	}

	// middle clicks
	if (events.type.M_MOUSE_BTN != m_btn) {
		if (events.type.M_MOUSE_BTN) {
			mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
		}
		else {
			mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
		}
		m_btn = events.type.M_MOUSE_BTN;
	}

	// middle mouse btn
	if (events.type.MOUSE_WHEEL) {
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, events.type.MOUSE_WHEEL, 0);
	}

	// handle keyboard events
	for (int addr = 0; events.type.KEYS[addr]._event; addr++) {

		INPUT input_event = {};
		memset(&input_event, 0, sizeof(INPUT));
		input_event.type = INPUT_KEYBOARD;
		input_event.ki.wVk = (WORD)events.type.KEYS[addr].key;

		if (events.type.KEYS[addr]._event == WM_KEYUP) {
			input_event.ki.dwFlags = KEYEVENTF_KEYUP;
		}

		SendInput(1, &input_event, sizeof(INPUT));
	}

	goto input;

	WSACleanup();
	return 0;
}
