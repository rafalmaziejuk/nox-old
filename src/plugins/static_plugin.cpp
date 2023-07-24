#include "plugins/static_plugin.h"
#include "renderer/renderer_plugin.h"

#if defined(NOX_BUILD_RENDERER_OPENGL)
#include "renderer/opengl/gl_plugin.h"
#endif

namespace NOX {

void *StaticPlugin::getProcedureAddress(std::string_view procedureName) const {
    if constexpr (isStaticLinking) {
#if defined(NOX_BUILD_RENDERER_OPENGL)
        if (m_name == RendererPlugin::glPluginName) {
            if (procedureName == RendererPlugin::allocateRendererProcedureName) {
                return GLPlugin::allocateRenderer;
            }
        }
#endif
    }

    NOX_ASSERT(true);
    return nullptr;
}

} // namespace NOX
