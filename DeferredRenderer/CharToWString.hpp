#pragma once
#include <string>
#include <cstring>
#include <cstdlib>

// Courtesy of http://stackoverflow.com/questions/3074776/how-to-convert-char-array-to-wchar-t-array
static std::wstring CharToWString(const char *text)
{
    const std::size_t size = std::strlen(text);
    std::wstring wstr;
    if (size > 0)
    {
        wstr.resize(size);
        MultiByteToWideChar(CP_ACP, 0, text, size, &wstr[0], size);
    }
    return wstr;
}
