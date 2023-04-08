#include "renderer/opengl/gl_renderer.h"

namespace NOX {

GLRenderer::GLRenderer() {
    NOX_LOG_INFO(OPENGL, "GLRenderer ctor");
}

GLRenderer::~GLRenderer() {
    NOX_LOG_INFO(OPENGL, "GLRenderer dtor");
}

} // namespace NOX
