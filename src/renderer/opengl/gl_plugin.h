#pragma once

#include "plugins/plugin.h"
#include "renderer/opengl/nox_opengl_export.h"

namespace NOX {

class Renderer;
struct RendererDescriptor;

extern "C" NOX_OPENGL_EXPORT Renderer *allocateRenderer(const RendererDescriptor &descriptor);

class GLPlugin final : public Plugin {
  public:
    using Plugin::Plugin;

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;
};

} // namespace NOX
