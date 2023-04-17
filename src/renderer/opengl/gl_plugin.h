#pragma once

#include "renderer/opengl/nox_opengl_export.h"

#include <cstdint>

namespace NOX {

class Renderer;
struct RendererDescriptor;

class GLPlugin {
  public:
    static constexpr auto pluginName = "OpenGL";

    [[nodiscard]] static Renderer *allocateRenderer(const RendererDescriptor &descriptor);
    static void deallocateRenderer(void *renderer);
};

extern "C" {
NOX_OPENGL_EXPORT void *allocateRenderer(const RendererDescriptor &descriptor);
NOX_OPENGL_EXPORT void deallocateRenderer(void *renderer);
NOX_OPENGL_EXPORT uint32_t getAbiCompatibilityIdentifier();
}

} // namespace NOX
