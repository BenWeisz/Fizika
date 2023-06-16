#pragma once

#include <string>

// ChatGPT
inline std::string Strings_FilenameWithoutExtension(const std::string& filename) {
    size_t lastDot = filename.find_last_of(".");
    if (lastDot != std::string::npos) {
        return filename.substr(0, lastDot);
    }
    return filename;
}

// Returns the start of the next integeral number
inline int Strings_GetStartOfNumber(const std::string& str, const int start) {
    if (start > str.length())
        return -1;

    int i = start;
    int v = -1;
    while (i < str.length()) {
        v = str[i] - '0';
        if (v >= 0 && v <= 9)
            return i;
        i++;
    }

    return -1;
}

// Returns the end of the current integeral number
inline int Strings_GetEndOfNumber(const std::string& str, const int start) {
    if (start >= str.length())
        return -1;

    int i = start;
    int v = str[i] - '0';
    if (v < 0 || v > 9)
        return -1;

    while (i < str.length()) {
        v = str[i] - '0';
        if (v < 0 || v > 9)
            return i;

        i++;
    }

    return i;
}