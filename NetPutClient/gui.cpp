
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);


bool register_window(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                      // Extended window style
        CLASS_NAME,             // Window class name
        L"Client",              // Window title
        WS_OVERLAPPEDWINDOW,    // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, // Window dimensions
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 1;
    }

    // Show and update the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return 0;
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        // Create a font
        HFONT hFont = CreateFont(
            16,                  // Height
            0,                   // Width
            0,                   // Escapement
            0,                   // Orientation
            FW_NORMAL,           // Weight
            FALSE,               // Italic
            FALSE,               // Underline
            0,                   // StrikeOut
            DEFAULT_CHARSET,     // Charset
            OUT_DEFAULT_PRECIS,  // OutPrecision
            CLIP_DEFAULT_PRECIS, // ClipPrecision
            DEFAULT_QUALITY,     // Quality
            DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
            L"Arial"             // Face name
        );

        if (hFont != NULL) {
            // Set the font for the main window
            SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
        }
        else {
            MessageBox(hwnd, L"Failed to create the font!", L"Error", MB_OK | MB_ICONERROR);
        }
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