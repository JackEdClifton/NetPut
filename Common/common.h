#pragma once

extern int port;

#define WINSOCK_DLL_ERROR	1
#define SOCK_SETUP_ERROR	2
int setup_winsock(SOCKET* sock);


struct key_event {
	UINT _event;
	WPARAM key;

	key_event(UINT _event, WPARAM key) {
		this->_event = _event;
		this->key = key;
	}
};


#define EVENT_BUFF_SIZE 512
#define KEY_EVENT_ARR_SIZE 5
union event_buff {
	char _buffer[EVENT_BUFF_SIZE];

	struct data {
		POINT POINT;		//  0 + 8	= 8
		short L_MOUSE_BTN;	//  8 + 2	= 10
		short R_MOUSE_BTN;	// 10 + 2	= 12
		short M_MOUSE_BTN;	// 12 + 2	= 14
		short MOUSE_WHEEL;	// 14 + 2	= 16
		key_event KEYS[KEY_EVENT_ARR_SIZE];	// 64 - 16	= 48 bytes free
	} type;
	
	event_buff() {}
};


extern event_buff events;

void event_buff_reset();
void event_buff_add_key(UINT key_event, WPARAM key);