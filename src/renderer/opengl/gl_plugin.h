#pragma once

#include "renderer/opengl/nox_opengl_export.h"

namespace NOX {

class Renderer;
struct RendererDescriptor;

namespace GLPlugin {

extern "C" {
NOX_OPENGL_EXPORT Renderer *allocateRenderer(const RendererDescriptor &descriptor);
}

} // namespace GLPlugin

} // namespace NOX
