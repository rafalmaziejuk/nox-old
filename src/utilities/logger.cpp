#include "utilities/logger.h"

NOX_DEFINE_LOG_CATEGORY(DEBUG);
NOX_DEFINE_LOG_CATEGORY(PLUGINS);

namespace NOX {

Logger::Logger() : m_consoleSink{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()} {
    m_consoleSink->set_pattern("[%T]%^[%n][%l] %v%$");
}

void Logger::registerCategory(LogCategory &logCategory) {
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

    Utilities::toLower(logCategory.categoryName);
    m_loggers[logCategory.categoryName] = std::make_unique<spdlog::logger>(logCategory.categoryName, m_consoleSink);
    m_loggers[logCategory.categoryName]->set_level(level);
}

} // namespace NOX
