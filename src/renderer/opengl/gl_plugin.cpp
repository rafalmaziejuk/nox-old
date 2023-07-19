#include "renderer/opengl/gl_plugin.h"
#include "renderer/opengl/gl_renderer.h"

namespace NOX::GLPlugin {

extern "C" {
Renderer *allocateRenderer(const RendererDescriptor &descriptor) {
    return new GLRenderer{descriptor};
}
}

} // namespace NOX::GLPlugin
