#include "opengl/gl_context.h"

namespace nox {

uint8_t GLContext::m_sInstanceCounter = 0u;

GLContext::GLContext() {
    m_sInstanceCounter++;
}

GLContext::~GLContext() {
    m_sInstanceCounter--;
}

} // namespace nox
