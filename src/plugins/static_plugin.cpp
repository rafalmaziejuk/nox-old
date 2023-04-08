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

StaticPlugin::StaticPlugin(std::string_view pluginName) : Plugin{pluginName} {
    NOX_LOG_DEBUG(PLUGINS, "Loaded [{}] static plugin", getPluginName());
}

StaticPlugin::~StaticPlugin() {
    NOX_LOG_DEBUG(PLUGINS, "Unloaded [{}] static plugin", getPluginName());
}

void *StaticPlugin::loadProcedure(std::string_view procedureName) const {
    NOX_LOG_TRACE(PLUGINS, "Loaded [{}] procedure from [{}] static plugin", procedureName, getPluginName());
    if (getPluginName() == Utilities::toLower(GLPlugin::pluginName)) {
        NOX_LOAD_OPENGL_PLUGIN_PROCEDURE(procedureName);
    }

    NOX_ASSERT_MSG(true, "Unable to load [{}] procedure from [{}] static plugin", procedureName, getPluginName());
    return nullptr;
}

} // namespace NOX
