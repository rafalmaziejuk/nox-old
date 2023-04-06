#pragma once

#include "utilities/logger.h"

#include <nox/renderer/renderer.h>

NOX_DECLARE_LOG_CATEGORY_EXTERN(OPENGL, TRACE);

namespace NOX {

class GLRenderer final : public Renderer {
  public:
    GLRenderer();
    ~GLRenderer();
};

} // namespace NOX
