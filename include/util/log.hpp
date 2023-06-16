#pragma once

#include <sstream>

// ChatGPT
template <typename... Args>
inline void LogError(Args&&... args) {
    std::ostringstream oss;
    ((oss << std::forward<Args>(args)), ...);  // Apply << operator to all arguments

    std::cout << "\033[1;41mERROR:\033[0m " << oss.str() << std::endl;
}

template <typename... Args>
inline void LogWarn(Args&&... args) {
    std::ostringstream oss;
    ((oss << std::forward<Args>(args)), ...);  // Apply << operator to all arguments

    std::cout << "\033[1;43mLOG:\033[0m " << oss.str() << std::endl;
}

template <typename... Args>
inline void LogSuccess(Args&&... args) {
    std::ostringstream oss;
    ((oss << std::forward<Args>(args)), ...);  // Apply << operator to all arguments

    std::cout << "\033[1;42mSUCCESS:\033[0m " << oss.str() << std::endl;
}