#pragma once

#include <windows.h>

#ifdef UIMODULE_EXPORTS
#define UIMODULE_API __declspec(dllexport)
#else
#define UIMODULE_API __declspec(dllimport)
#endif

extern "C" UIMODULE_API void InitializeUI(HINSTANCE hInstance, int nCmdShow);
extern "C" UIMODULE_API void ShowSecondWindow();
