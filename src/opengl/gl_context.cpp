#include "opengl/gl_context.h"

namespace nox {

uint8_t GLContext::m_sContextCounter = 0u;

GLContext::GLContext() {
    m_sContextCounter++;
}

GLContext::~GLContext() {
    m_sContextCounter--;
}

} // namespace nox
