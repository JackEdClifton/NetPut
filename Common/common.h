#pragma once

extern int port;

#define WINSOCK_DLL_ERROR	1
#define SOCK_SETUP_ERROR	2
int setup_winsock(SOCKET* sock);


#define EVENT_BUFF_SIZE 32
union event_buff {
	char _buffer[EVENT_BUFF_SIZE];

	struct {
		POINT POINT;
		short L_MOUSE_BTN;
		short R_MOUSE_BTN;
		short MOUSE_WHEEL;
	} type;
};

extern event_buff events;