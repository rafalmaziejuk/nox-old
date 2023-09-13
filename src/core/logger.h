#pragma once

#include "core/singleton.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define NOX_LOG_IMPL(verbosity, category, message, ...) \
    Logger::instance().getLogger().verbosity("[" #category "] " message, ##__VA_ARGS__)

#define NOX_LOG_TRACE(category, message, ...) NOX_LOG_IMPL(trace, category, message, ##__VA_ARGS__)
#define NOX_LOG_DEBUG(category, message, ...) NOX_LOG_IMPL(debug, category, message, ##__VA_ARGS__)
#define NOX_LOG_INFO(category, message, ...) NOX_LOG_IMPL(info, category, message, ##__VA_ARGS__)
#define NOX_LOG_WARN(category, message, ...) NOX_LOG_IMPL(warn, category, message, ##__VA_ARGS__)
#define NOX_LOG_ERROR(category, message, ...) NOX_LOG_IMPL(error, category, message, ##__VA_ARGS__)
#define NOX_LOG_CRITICAL(category, message, ...) NOX_LOG_IMPL(critical, category, message, ##__VA_ARGS__)

namespace NOX {

class Logger : public Singleton<Logger> {
  public:
    Logger() {
        auto consoleSink{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
        consoleSink->set_pattern("[%T]%^[%l][%n]%v%$");

        m_logger = std::make_unique<spdlog::logger>("NOX", consoleSink);
        m_logger->set_level(spdlog::level::trace);
    }

    spdlog::logger &getLogger() const { return *m_logger; }

  private:
    std::unique_ptr<spdlog::logger> m_logger{nullptr};
};

} // namespace NOX
