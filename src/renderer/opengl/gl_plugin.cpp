#include "renderer/renderer_plugin.h"
#include "renderer/opengl/gl_plugin.h"
#include "renderer/opengl/gl_renderer.h"

namespace NOX {

extern "C" Renderer *allocateRenderer(const RendererDescriptor &descriptor) {
    NOX_ASSERT(descriptor.api != RendererAPI::OPENGL);

    return new GLRenderer{descriptor};
}

void *GLPlugin::getProcedureAddress(std::string_view procedureName) const {
    NOX_ASSERT(procedureName.empty());

    if (procedureName == RendererPlugin::allocateRendererProcedureName) {
        return reinterpret_cast<void *>(allocateRenderer);
    }

    NOX_ASSERT(true);
    return nullptr;
}

} // namespace NOX
