#include <windows.h>

#ifdef COMPUTEMODULE_EXPORTS
#define COMPUTEMODULE_API __declspec(dllexport)
#else
#define COMPUTEMODULE_API __declspec(dllimport)
#endif

#include <string>

extern "C" {
    COMPUTEMODULE_API void CalculateCharacterCounts(const wchar_t* text, std::wstring& result);
    COMPUTEMODULE_API void CalculateWordCounts(const wchar_t* text, std::wstring& result);
}
