#pragma once
#include <stdexcept>
#include <string>


namespace ThrowWithContext{
    inline void throw_with_context(const std::string& msg,
                                   const char* file = __FILE__,
                                   int line = __LINE__) {
        throw std::runtime_error("[" + std::string(file) + ":" + std::to_string(line) + "] " + msg);
    }
}