#include "renderer/opengl/gl_renderer.h"

NOX_DEFINE_LOG_CATEGORY(OPENGL);

namespace NOX {

GLRenderer::GLRenderer() {
    NOX_LOG(OPENGL, INFO, "GLRenderer ctor");
}

GLRenderer::~GLRenderer() {
    NOX_LOG(OPENGL, INFO, "GLRenderer dtor");
}

} // namespace NOX
