#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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

class Log {
  public:
    Log();

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

    static Log &get() {
        static Log instance;
        return instance;
    }

  private:
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> m_consoleSink{nullptr};
    std::unordered_map<std::string, std::unique_ptr<spdlog::logger>> m_loggers{};
};

} // namespace NOX
