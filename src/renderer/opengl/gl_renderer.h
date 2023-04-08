#pragma once

#include "utilities/logger.h"

#include <nox/renderer/renderer.h>

namespace NOX {

class GLRenderer final : public Renderer {
  public:
    GLRenderer();
    ~GLRenderer();
};

} // namespace NOX
