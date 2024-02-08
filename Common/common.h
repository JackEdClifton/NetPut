#pragma once

extern int port;

#define WINSOCK_DLL_ERROR	1
#define SOCK_SETUP_ERROR	2
int setup_winsock(SOCKET* sock);


#define _POINT_SIZE 32
union _point {
	POINT POINT;
	char buffer[_POINT_SIZE];
};