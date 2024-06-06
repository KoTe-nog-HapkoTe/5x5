#include <windows.h>

#pragma comment(lib, "C:/Users/ilis2/OneDrive/Рабочий стол/data/Visual/x64/Debug/Visual.lib")

HINSTANCE hInst;

extern "C" __declspec(dllimport) void InitializeUI(HINSTANCE hInstance, int nCmdShow);
extern "C" __declspec(dllimport) void ShowSecondWindow();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    InitializeUI(hInstance, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}