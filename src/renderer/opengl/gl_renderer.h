#pragma once

#include <nox/renderer/renderer.h>

namespace NOX {

class GLContext;

class GLRenderer final : public Renderer {
  public:
    GLRenderer(const RendererDescriptor &descriptor);

  private:
    std::shared_ptr<GLContext> m_context{nullptr};

    RendererDescriptor m_descriptor;
};

} // namespace NOX
