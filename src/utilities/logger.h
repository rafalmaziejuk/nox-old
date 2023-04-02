#pragma once

#include <nox/core/singleton.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define NOX_DECLARE_LOG_CATEGORY_EXTERN(categoryName, defaultVerbosity)                              \
    extern struct LogCategory##categoryName : public NOX::LogCategory {                              \
        LogCategory##categoryName() : LogCategory(#categoryName, NOX::Verbosity::defaultVerbosity) { \
            NOX::Logger::instance().registerCategory(*this);                                         \
        }                                                                                            \
    } categoryName;

#define NOX_DEFINE_LOG_CATEGORY(categoryName) \
    LogCategory##categoryName categoryName{};

#define NOX_LOG(categoryName, verbosity, ...) \
    NOX::Logger::instance().log<NOX::Verbosity::verbosity>(categoryName, __VA_ARGS__)

namespace NOX {

enum class Verbosity {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL
};

struct LogCategory {
    LogCategory(std::string_view name, Verbosity verbosity) : categoryName{name},
                                                              defaultVerbosity{verbosity} {}

    std::string categoryName;
    Verbosity defaultVerbosity;
};

class NOX_EXPORT Logger : public Singleton<Logger> {
  public:
    Logger();

    void registerCategory(LogCategory &logCategory);

    template <Verbosity verbosity, typename... Args>
    void log(const LogCategory &logCategory, Args &&...args) {
        switch (verbosity) {
        case Verbosity::TRACE:
            m_loggers[logCategory.categoryName]->trace(std::forward<Args>(args)...);
            break;

        case Verbosity::DEBUG:
            m_loggers[logCategory.categoryName]->debug(std::forward<Args>(args)...);
            break;

        case Verbosity::INFO:
            m_loggers[logCategory.categoryName]->info(std::forward<Args>(args)...);
            break;

        case Verbosity::WARN:
            m_loggers[logCategory.categoryName]->warn(std::forward<Args>(args)...);
            break;

        case Verbosity::ERROR:
            m_loggers[logCategory.categoryName]->error(std::forward<Args>(args)...);
            break;

        case Verbosity::CRITICAL:
            m_loggers[logCategory.categoryName]->critical(std::forward<Args>(args)...);
            break;
        }
    }

  private:
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> m_consoleSink{nullptr};
    std::unordered_map<std::string, std::unique_ptr<spdlog::logger>> m_loggers{};
};

} // namespace NOX

NOX_DECLARE_LOG_CATEGORY_EXTERN(DEBUG, ERROR);
