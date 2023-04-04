#include "renderer/opengl/gl_plugin.h"
#include "renderer/opengl/gl_renderer.h"
#include "plugins/plugin_abi_compatibility_identifier.h"

namespace NOX {

Renderer *GLPlugin::allocateRenderer() {
    return new GLRenderer;
}

void GLPlugin::deallocateRenderer(void *renderer) {
    auto *rendererToDeallocate = static_cast<GLRenderer *>(renderer);
    delete rendererToDeallocate;
}

} // namespace NOX

extern "C" {
void *allocateRenderer() {
    return new NOX::GLRenderer;
}

void deallocateRenderer(void *renderer) {
    auto *rendererToDeallocate = static_cast<NOX::GLRenderer *>(renderer);
    delete rendererToDeallocate;
}

uint32_t getAbiCompatibilityIdentifier() {
    return NOX_ABI_COMPATIBILITY_IDENTIFIER;
}
}
