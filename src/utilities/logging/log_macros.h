#pragma once

#include "utilities/logging/log.h"

#define NOX_DECLARE_LOG_CATEGORY_EXTERN(categoryName, defaultVerbosity)                              \
    extern struct LogCategory##categoryName : public NOX::LogCategory {                              \
        LogCategory##categoryName() : LogCategory(#categoryName, NOX::Verbosity::defaultVerbosity) { \
            NOX::Log::get().registerLogger(*this);                                                   \
        }                                                                                            \
    } categoryName;

#define NOX_DEFINE_LOG_CATEGORY(categoryName) \
    LogCategory##categoryName categoryName{};

#define NOX_LOG(categoryName, verbosity, ...) \
    NOX::Log::get().log<NOX::Verbosity::verbosity>(categoryName, __VA_ARGS__)
