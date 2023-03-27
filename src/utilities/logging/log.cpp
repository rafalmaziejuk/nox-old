#include "utilities/string_helper.h"
#include "utilities/logging/log.h"

namespace NOX {

Log::Log() : m_consoleSink{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()} {
    m_consoleSink->set_pattern("[%T]%^[%n][%l] %v%$");
}

void Log::registerCategory(LogCategory &logCategory) {
    spdlog::level::level_enum level{};
    switch (logCategory.defaultVerbosity) {
    case Verbosity::TRACE:
        level = spdlog::level::trace;
        break;
    case Verbosity::DEBUG:
        level = spdlog::level::debug;
        break;
    case Verbosity::INFO:
        level = spdlog::level::info;
        break;
    case Verbosity::WARN:
        level = spdlog::level::warn;
        break;
    case Verbosity::ERROR:
        level = spdlog::level::err;
        break;
    case Verbosity::CRITICAL:
        level = spdlog::level::critical;
        break;
    }

    StringHelper::toLower(logCategory.categoryName);
    m_loggers[logCategory.categoryName] = std::make_unique<spdlog::logger>(logCategory.categoryName, m_consoleSink);
    m_loggers[logCategory.categoryName]->set_level(level);
}

} // namespace NOX
