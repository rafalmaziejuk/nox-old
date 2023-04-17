#include "renderer/opengl/gl_plugin.h"
#include "renderer/opengl/gl_renderer.h"
#include "plugins/plugin_abi_compatibility_identifier.h"

namespace NOX {

Renderer *GLPlugin::allocateRenderer(const RendererDescriptor &descriptor) {
    return new GLRenderer{descriptor};
}

void GLPlugin::deallocateRenderer(void *renderer) {
    auto *rendererToDeallocate = static_cast<GLRenderer *>(renderer);
    delete rendererToDeallocate;
}

extern "C" {
void *allocateRenderer(const RendererDescriptor &descriptor) {
    return new GLRenderer{descriptor};
}

void deallocateRenderer(void *renderer) {
    auto *rendererToDeallocate = static_cast<GLRenderer *>(renderer);
    delete rendererToDeallocate;
}

uint32_t getAbiCompatibilityIdentifier() {
    return NOX_ABI_COMPATIBILITY_IDENTIFIER;
}
}

} // namespace NOX
