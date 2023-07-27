#pragma once

#include <nox/export.h>

namespace NOX {

class NOX_EXPORT Resource {
  public:
    Resource(const Resource &) = delete;
    Resource &operator=(const Resource &) = delete;
    virtual ~Resource();

  protected:
    Resource() = default;
};

} // namespace NOX
