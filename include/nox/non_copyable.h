#pragma once

#include <nox/export.h>

namespace NOX {

class NOX_EXPORT NonCopyable {
  public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
    NonCopyable(NonCopyable &&) = delete;
    NonCopyable &operator=(NonCopyable &&) = delete;

  protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

} // namespace NOX
