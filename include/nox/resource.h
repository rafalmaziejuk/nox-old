#pragma once

#include <nox/export.h>

namespace NOX {

class NOX_EXPORT Resource {
  public:
    Resource(const Resource &) = delete;
    Resource &operator=(const Resource &) = delete;
    Resource(Resource &&) = delete;
    Resource &operator=(Resource &&) = delete;
    virtual ~Resource() = default;

  protected:
    Resource() = default;
};

} // namespace NOX
