
#include <iostream>
#include <winsock2.h>

#include "common.h"

int port = 30018;

event_buff events;

int setup_winsock(SOCKET* sock) {

	// setup DLL
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr) {
		std::cout << "Error: Winsock dll not found\n";
		return WINSOCK_DLL_ERROR;
	}

	// setup socket
	*sock = INVALID_SOCKET;
	*sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*sock == INVALID_SOCKET) {
		std::cout << "Error: Could not setup socket" << WSAGetLastError() << std::endl;
		WSACleanup();
		return SOCK_SETUP_ERROR;
	}

	return 0;
}


static int event_keys_addr = 0;

void event_buff_reset() {
	event_keys_addr = 0;
	events.type.MOUSE_WHEEL = 0;

	for (int i = KEY_EVENT_ARR_SIZE - 1; i >= 0; i--) {
		events.type.KEYS[event_keys_addr].key = 0;
		events.type.KEYS[event_keys_addr]._event = 0;
	}
}


void event_buff_add_key(UINT _key_event, WPARAM key) {

	// ignore F7
	if (key == 118) {
		return;
	}

	if (event_keys_addr < KEY_EVENT_ARR_SIZE) {
		events.type.KEYS[event_keys_addr] = key_event(_key_event, key);
		event_keys_addr++;
	}

	// else do nothing
	// it will be rare to use up
	// all 5 slots with the frequency
	// inputs are transfred
	// if we ever do get to this pretend
	// the input did not exist
	//
	// because we are not people who make
	// proper queues to handle this properly
	// for no reason when user is at fault
}