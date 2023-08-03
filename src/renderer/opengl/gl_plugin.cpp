#include "plugins/static_plugin.h"
#include "renderer/renderer_plugin.h"
#include "renderer/opengl/gl_plugin.h"
#include "renderer/opengl/gl_renderer.h"

namespace NOX {

namespace {

constexpr std::array<std::pair<const char *, void *>, 1> glPluginProcedures{{
    {RendererPlugin::allocateRendererProcedureName, GLPlugin::allocateRenderer},
}};

} // namespace

void *StaticPlugin::getProcedureAddress(std::string_view procedureName) const {
    if constexpr (isStaticLinking) {
        auto iterator = std::find_if(glPluginProcedures.begin(), glPluginProcedures.end(), [procedureName](const auto &element) {
            return (procedureName == std::string(element.first));
        });
        if (iterator != glPluginProcedures.end()) {
            return iterator->second;
        }

        NOX_ASSERT(true);
        return nullptr;
    }
}

extern "C" {
Renderer *GLPlugin::allocateRenderer(const RendererDescriptor &descriptor) {
    return new GLRenderer{descriptor};
}
}

} // namespace NOX
