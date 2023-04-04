#include "plugins/plugin_procedure_names.h"
#include "plugins/static_plugin.h"

#if defined(NOX_STATIC)
#define NOX_LOAD_RENDERER_PLUGIN_PROCEDURE_IMPL(pluginName, procedureName)  \
    if (procedureName == RendererPlugin::allocateRendererProcedureName) {   \
        return reinterpret_cast<void *>(pluginName::allocateRenderer);      \
    }                                                                       \
    if (procedureName == RendererPlugin::deallocateRendererProcedureName) { \
        return reinterpret_cast<void *>(pluginName::deallocateRenderer);    \
    }
#else
#define NOX_LOAD_RENDERER_PLUGIN_PROCEDURE_IMPL(pluginName, procedureName) (void)procedureName
#endif

#if defined(NOX_BUILD_RENDERER_OPENGL)
#include "renderer/opengl/gl_plugin.h"
#define NOX_LOAD_OPENGL_PLUGIN_PROCEDURE(procedureName) \
    NOX_LOAD_RENDERER_PLUGIN_PROCEDURE_IMPL(GLPlugin, procedureName)
#endif

namespace NOX {

StaticPlugin::StaticPlugin(std::string_view pluginName) : Plugin{pluginName} {}

void *StaticPlugin::loadProcedure(std::string_view procedureName) const {
    if (getPluginName() == Utilities::toLower(GLPlugin::pluginName)) {
        NOX_LOAD_OPENGL_PLUGIN_PROCEDURE(procedureName);
    }

    NOX_ASSERT_IF(true, "Unable to load \"{0}\" procedure", procedureName);
    return nullptr;
}

} // namespace NOX
