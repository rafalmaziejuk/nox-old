#pragma once

#if defined(NOX_STATIC)
static constexpr auto staticEnabled = true;
#else
static constexpr auto staticEnabled = false;
#endif

#if defined(NOX_DEBUG)
static constexpr auto debugEnabled = true;
#else
static constexpr auto debugEnabled = false;
#endif

#if (defined(__GNUC__) || defined(NOX_UNIX))
static constexpr auto unixEnvironment = true;
#else
static constexpr auto unixEnvironment = false;
#endif

template <typename TargetType, typename SourceType>
const TargetType *downcast(const SourceType &object) {
    return dynamic_cast<const TargetType *>(&object);
}
