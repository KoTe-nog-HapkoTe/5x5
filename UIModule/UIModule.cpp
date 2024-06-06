// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

#include <windows.h>
#include "UIModule.h"
#include <string>
#include <sstream>
#include <map>
#include <iostream>

// Определение глобальных переменных
HINSTANCE hInst;
HWND hwndMain, hwndEdit1, hwndCountDisplay, hwndButton, hwndSecond, hwndEdit2;

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SecondWndProc(HWND, UINT, WPARAM, LPARAM);

void InitializeUI(HINSTANCE hInstance, int nCmdShow) {
    WNDCLASS wcMain = { 0 };
    WNDCLASS wcSecond = { 0 };
    hInst = hInstance;

    wcMain.style = CS_HREDRAW | CS_VREDRAW;
    wcMain.lpfnWndProc = MainWndProc;
    wcMain.hInstance = hInstance;
    wcMain.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcMain.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcMain.lpszClassName = TEXT("MainWindow");

    if (!RegisterClass(&wcMain)) return;

    hwndMain = CreateWindow(wcMain.lpszClassName, TEXT("Main Window"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);
    if (!hwndMain) return;

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    wcSecond.style = CS_HREDRAW | CS_VREDRAW;
    wcSecond.lpfnWndProc = SecondWndProc;
    wcSecond.hInstance = hInstance;
    wcSecond.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcSecond.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcSecond.lpszClassName = TEXT("SecondWindow");

    if (!RegisterClass(&wcSecond)) return;
}

void ShowSecondWindow() {
    if (!hwndSecond) {
        hwndSecond = CreateWindow(TEXT("SecondWindow"), TEXT("Second Window"), WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, hwndMain, NULL, hInst, NULL);
        ShowWindow(hwndSecond, SW_SHOW);
        UpdateWindow(hwndSecond);
    }
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hwndEdit1 = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL,
            10, 10, 360, 80, hwnd, (HMENU)1, hInst, NULL);
        hwndCountDisplay = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_READONLY,
            10, 100, 360, 150, hwnd, (HMENU)2, hInst, NULL);
        hwndButton = CreateWindow(TEXT("button"), TEXT("Open Second Window"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 260, 360, 30, hwnd, (HMENU)3, hInst, NULL);
    }
                  break;

    case WM_COMMAND: {
        if (LOWORD(wParam) == 1 && HIWORD(wParam) == EN_CHANGE) {
            if (hwndSecond) {
                int len = GetWindowTextLength(hwndEdit1) + 1;
                TCHAR* text = new TCHAR[len];
                GetWindowText(hwndEdit1, text, len);

                typedef void (*CalculateCharacterCountsFunc)(const wchar_t*, std::wstring&);
                typedef void (*CalculateWordCountsFunc)(const wchar_t*, std::wstring&);
                HMODULE hModule = LoadLibrary(TEXT("ComputeModule.dll"));
                if (hModule) {
                    CalculateCharacterCountsFunc CalculateCharacterCounts = (CalculateCharacterCountsFunc)GetProcAddress(hModule, "CalculateCharacterCounts");
                    CalculateWordCountsFunc CalculateWordCounts = (CalculateWordCountsFunc)GetProcAddress(hModule, "CalculateWordCounts");
                    if (CalculateCharacterCounts && CalculateWordCounts) {
                        std::wstring charResult;
                        std::wstring wordResult;
                        CalculateCharacterCounts(text, charResult);
                        CalculateWordCounts(text, wordResult);

                        std::wstring combinedResult =  charResult + L"\r\n Количество слов:\r\n" + wordResult;
                        SetWindowText(hwndEdit2, combinedResult.c_str());
                        SetWindowText(hwndCountDisplay, combinedResult.c_str());
                    }
                    else {
                        MessageBox(hwnd, TEXT("Failed to get the function address"), TEXT("Error"), MB_OK);
                    }
                    FreeLibrary(hModule);
                }
                else {
                    MessageBox(hwnd, TEXT("Failed to load ComputeModule.dll"), TEXT("Error"), MB_OK);
                }

                delete[] text;
            }
        }
        else if (LOWORD(wParam) == 3) {
            ShowSecondWindow();
        }
    }
                   break;

    case WM_SIZE: {
        MoveWindow(hwndEdit1, 0, 0, LOWORD(lParam), HIWORD(lParam) - 190, TRUE);
        RECT rcClient;
        GetClientRect(hwnd, &rcClient);
        SetWindowPos(hwndCountDisplay, NULL, rcClient.left, rcClient.bottom - 180, rcClient.right - rcClient.left, 150, SWP_NOZORDER);
        SetWindowPos(hwndButton, NULL, rcClient.left, rcClient.bottom - 30, rcClient.right - rcClient.left, 30, SWP_NOZORDER);
    }
                break;

    case WM_GETMINMAXINFO: {
        MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;
        pMinMax->ptMinTrackSize.x = 400;
        pMinMax->ptMinTrackSize.y = 300;
    }
                         break;

    case WM_DESTROY: {
        PostQuitMessage(0);
    }
                   break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK SecondWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hwndEdit2 = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_READONLY,
            10, 10, 360, 80, hwnd, (HMENU)2, hInst, NULL);
    }
                  break;

    case WM_SIZE: {
        MoveWindow(hwndEdit2, 10, 10, LOWORD(lParam) - 20, HIWORD(lParam) - 20, TRUE);
    }
                break;

    case WM_GETMINMAXINFO: {
        MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;
        pMinMax->ptMinTrackSize.x = 400;
        pMinMax->ptMinTrackSize.y = 200;
    }
                         break;

    case WM_DESTROY: {
        hwndSecond = NULL;
        DestroyWindow(hwnd);
    }
                   break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
