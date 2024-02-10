
#include <Windows.h>
#include "../Common/common.h"

#define IDM_IP_ADDR 1
#define IDM_PORT    2
#define IDC_EDIT_IP	3
#define IDM_CONN	4

extern SOCKET sock;
int connect_socket(SOCKET&);

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);


bool register_window(HWND* hwnd, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	const wchar_t CLASS_NAME[] = L"MyWindowClass";

	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window
	*hwnd = CreateWindowEx(
		0,                      // Extended window style
		CLASS_NAME,             // Window class name
		L"Client",              // Window title
		WS_OVERLAPPEDWINDOW,    // Window style
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, // Window dimensions
		NULL, NULL, hInstance, NULL
	);

	if (*hwnd == NULL) {
		return 1;
	}

	// add menu bar
	HMENU hMenu = CreateMenu();
	HMENU hSetup = CreatePopupMenu();


	AppendMenu(hSetup, MF_STRING, IDM_CONN, L"&Connect");
	AppendMenu(hSetup, MF_STRING, IDM_IP_ADDR, L"&IP Address");
	AppendMenu(hSetup, MF_STRING, IDM_PORT, L"&Port");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSetup, L"&Setup");


	// Show and update the window
	ShowWindow(*hwnd, nCmdShow);
	UpdateWindow(*hwnd);
	SetMenu(*hwnd, hMenu);

	ShowWindow(*hwnd, SW_SHOWNORMAL);
	UpdateWindow(*hwnd);

	return 0;
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE: {
		// Create a font
		HFONT hFont = CreateFont(
			16, 0, 0, 0,					// height width escapement orientation
			FW_NORMAL, FALSE, FALSE, 0,		// weight italic underline strikeout
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,	// charset outPrecision
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,	// clipPrecision quality
			DEFAULT_PITCH | FF_DONTCARE, L"Arial"	// pitch and family
		);

		if (hFont != NULL) {
			SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		else {
			MessageBox(hwnd, L"Failed to create the font!", L"Error", MB_OK | MB_ICONERROR);
		}

		// create text
		CreateWindow(L"STATIC", L"Press F7 to toggle input",
			WS_CHILD | WS_VISIBLE,
			5, 2, 160, 20,
			hwnd, nullptr,
			GetModuleHandle(nullptr), nullptr);

		CreateWindow(L"STATIC", L"Running",
			WS_CHILD | WS_VISIBLE,
			180, 2, 100, 20,
			hwnd, (HMENU)50,
			GetModuleHandle(nullptr), nullptr);

		break;
	}


	case WM_COMMAND: {

		switch (LOWORD(wParam)) {
		case IDM_IP_ADDR:
		case IDM_PORT:
			MessageBox(NULL, L"Not Implemented!", L"Error!", MB_OK | MB_ICONERROR);
			break;

		case IDM_CONN:
			if (connect_socket(sock)) {
				MessageBox(NULL, L"Could not connect to other PC", L"NetPut", MB_OK | MB_ICONWARNING);
			}
			break;
		}
		break;
	}

	case WM_KEYDOWN: 
	case WM_KEYUP:
	{
		event_buff_add_key(uMsg, wParam);
		break;
	}

	case WM_MOUSEWHEEL: {
		events.type.MOUSE_WHEEL = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}