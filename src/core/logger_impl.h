#pragma once

#include "core/singleton.h"
#include "core/utilities.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <memory>
#include <string_view>

namespace NOX {

enum class LogLevel : uint8_t;

class LoggerImpl {
    static constexpr auto loggerName = "nox";

  public:
    static void initialize();

    static void setLevel(LogLevel level);
    static void setLogFile(std::string_view filename);

    template <typename... Args>
    static void log(spdlog::level::level_enum level, std::string_view tag, std::string_view message, Args &&...args) {
        constexpr auto pattern = "[nox.{0}]{1}";
        auto logger = spdlog::get(loggerName);
        auto format = fmt::format(pattern, Utilities::toLower(tag.data()), message);

        switch (level) {
        case spdlog::level::trace:
            logger->trace(format, std::forward<Args>(args)...);
            break;
        case spdlog::level::debug:
            logger->debug(format, std::forward<Args>(args)...);
            break;
        case spdlog::level::info:
            logger->info(format, std::forward<Args>(args)...);
            break;
        case spdlog::level::warn:
            logger->warn(format, std::forward<Args>(args)...);
            break;
        case spdlog::level::err:
            logger->error(format, std::forward<Args>(args)...);
            break;
        case spdlog::level::critical:
            logger->critical(format, std::forward<Args>(args)...);
            break;
        }
    }
};

class FunctionTracer final {
  public:
    FunctionTracer(std::string_view tag, std::string_view functionName) : m_tag{tag},
                                                                          m_functionName{functionName} {
        LoggerImpl::log(spdlog::level::trace, m_tag, ">> {}", m_functionName);
    }

    ~FunctionTracer() {
        LoggerImpl::log(spdlog::level::trace, m_tag, "<< {}", m_functionName);
    }

  private:
    std::string m_tag;
    std::string m_functionName;
};

} // namespace NOX

#define NOX_LOG_TRACE_DECLARE(tag) NOX::FunctionTracer tag##FunctionTracer(#tag, __FUNCTION__)
#define NOX_LOG_TRACE_DECLARE_ONCE(tag) \
    static bool once = true;            \
    if (once) {                         \
        NOX_LOG_TRACE_DECLARE(tag);     \
        once = false;                   \
    }

#define NOX_LOG_DEBUG(tag, message, ...) NOX::LoggerImpl::log(spdlog::level::debug, #tag, message, __VA_ARGS__)
#define NOX_LOG_INFO(tag, message, ...) NOX::LoggerImpl::log(spdlog::level::info, #tag, message, __VA_ARGS__)
#define NOX_LOG_WARN(tag, message, ...) NOX::LoggerImpl::log(spdlog::level::warn, #tag, message, __VA_ARGS__)
#define NOX_LOG_ERROR(tag, message, ...) NOX::LoggerImpl::log(spdlog::level::err, #tag, message, __VA_ARGS__)
#define NOX_LOG_CRITICAL(tag, message, ...) NOX::LoggerImpl::log(spdlog::level::critical, #tag, message, __VA_ARGS__)
