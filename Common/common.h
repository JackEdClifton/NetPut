#pragma once

extern int port;

#define WINSOCK_DLL_ERROR	1
#define SOCK_SETUP_ERROR	2
bool setup_winsock(SOCKET sock);