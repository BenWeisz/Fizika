#pragma once

#include <iostream>
#include <sstream>

// ChatGPT
template <typename... Args>
inline void Log_Error(Args&&... args) {
    std::ostringstream oss;
    ((oss << std::forward<Args>(args)), ...);  // Apply << operator to all arguments

    std::cout << "\033[1;41mERROR:\033[0m " << oss.str() << std::endl;
}

template <typename... Args>
inline void Log_Warn(Args&&... args) {
    std::ostringstream oss;
    ((oss << std::forward<Args>(args)), ...);  // Apply << operator to all arguments

    std::cout << "\033[1;43mWARNING:\033[0m " << oss.str() << std::endl;
}

template <typename... Args>
inline void Log_Success(Args&&... args) {
    std::ostringstream oss;
    ((oss << std::forward<Args>(args)), ...);  // Apply << operator to all arguments

    std::cout << "\033[1;42mSUCCESS:\033[0m " << oss.str() << std::endl;
}