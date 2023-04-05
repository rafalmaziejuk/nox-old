#pragma once

#include "renderer/opengl/nox_opengl_export.h"

#include <cstdint>

namespace NOX {

class Renderer;

class GLPlugin {
  public:
    static constexpr auto pluginName = "OpenGL";

    [[nodiscard]] static Renderer *allocateRenderer();
    static void deallocateRenderer(void *renderer);
};

} // namespace NOX

extern "C" {
NOX_OPENGL_EXPORT void *allocateRenderer();
NOX_OPENGL_EXPORT void deallocateRenderer(void *renderer);
NOX_OPENGL_EXPORT uint32_t getAbiCompatibilityIdentifier();
}