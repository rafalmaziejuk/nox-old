#include "core/logger_impl.h"

#include <nox/logger.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace NOX {

namespace {

spdlog::level::level_enum mapToSpdlogLevel(LogLevel level) {
    switch (level) {
    case LogLevel::OFF:
        return spdlog::level::off;

    case LogLevel::TRACE:
        return spdlog::level::trace;

    case LogLevel::DEBUG:
        return spdlog::level::debug;

    case LogLevel::INFO:
        return spdlog::level::info;

    case LogLevel::WARN:
        return spdlog::level::warn;

    case LogLevel::ERROR:
        return spdlog::level::err;

    case LogLevel::CRITICAL:
        return spdlog::level::critical;

    default:
        return spdlog::level::off;
    }
}

} // namespace

void LoggerImpl::initialize() {
    constexpr auto defaultPattern = "%^[%T]%v%$";
#if defined(NOX_DEBUG)
    constexpr auto defaultLogLevel = spdlog::level::trace;
#else
    constexpr auto defaultLogLevel = spdlog::level::off;
#endif

    auto logger = std::make_shared<spdlog::logger>(loggerName);
    logger->sinks().emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->set_pattern(defaultPattern);
    logger->set_level(defaultLogLevel);

    spdlog::register_logger(logger);
}

void LoggerImpl::setLevel(LogLevel level) {
    auto logger = spdlog::get(loggerName);
    logger->set_level(mapToSpdlogLevel(level));
}

void LoggerImpl::setLogFile(std::string_view filename) {
    constexpr auto defaultPattern = "[%T][%l]%v";

    auto logger = spdlog::get(loggerName);
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename.data(), true);
    fileSink->set_pattern(defaultPattern);
    logger->sinks().push_back(std::move(fileSink));
}

} // namespace NOX
