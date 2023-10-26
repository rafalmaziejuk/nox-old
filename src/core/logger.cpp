#include "core/logger_impl.h"

#include <nox/logger.h>

namespace NOX {
void Logger::initialize() {
    LoggerImpl::initialize();
}

void Logger::setLevel(LogLevel level) {
    LoggerImpl::setLevel(level);
}

void Logger::setLogFile(std::string_view filename) {
    LoggerImpl::setLogFile(filename);
}

} // namespace NOX
