// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

#define COMPUTEMODULE_EXPORTS
#include "framework.h"
#include <map>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>

void CalculateCharacterCounts(const wchar_t* text, std::wstring& result) {
    std::map<wchar_t, int> charCounts;
    const wchar_t* ptr = text;

    while (*ptr) {
        charCounts[*ptr]++;
        ptr++;
    }

    result.clear();
    for (const auto& pair : charCounts) {
        result += pair.first;
        result += L": ";
        result += std::to_wstring(pair.second);
        result += L"\r\n";
    }
}

void CalculateWordCounts(const wchar_t* text, std::wstring& result) {
    int wordCounts = 0;
    std::wstringstream ss(text);
    std::wstring word;

    while (ss >> word) {
        wordCounts++;
    }
    result += std::to_wstring(wordCounts);
}
