#pragma once

#include <string>

// ChatGPT
inline std::string FilenameWithoutExtension(const std::string& filename) {
    size_t lastDot = filename.find_last_of(".");
    if (lastDot != std::string::npos) {
        return filename.substr(0, lastDot);
    }
    return filename;
}