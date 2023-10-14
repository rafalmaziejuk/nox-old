#pragma once

#include <nox/export.h>

#include <string_view>

namespace NOX {

enum class LogLevel : uint8_t {
    OFF,
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL
};

class NOX_EXPORT Logger {
  public:
    static void initialize();

    static void setLevel(LogLevel level);
    static void setLogFile(std::string_view filename);
};

} // namespace NOX
