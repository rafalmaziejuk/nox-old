#include "plugins/plugin.h"
#include "plugins/plugin_manager.h"
#include "renderer/renderer_plugin.h"

#include <nox/renderer.h>

namespace NOX {

Renderer::~Renderer() = default;

std::unique_ptr<Renderer, RendererDeleter> Renderer::create(const RendererDescriptor &descriptor) {
    NOX_ASSERT(descriptor.api >= RendererAPI::MAX);

    auto &pluginManager = PluginManager::instance();
    const auto *rendererName = RendererPlugin::getRendererName(descriptor.api);
    const auto rendererPluginName = Plugin::createFilename(rendererName, Config::gccEnabled, Config::debugEnabled);

    PluginHandle pluginHandle = 0u;
    if (pluginManager.isPluginRegistered(rendererPluginName)) {
        pluginHandle = pluginManager.getPluginHandle(rendererPluginName);
    } else {
        auto *plugin = RendererPlugin::load(descriptor.api, rendererPluginName);
        pluginHandle = pluginManager.registerPlugin(plugin);
    }
    NOX_ASSERT(pluginHandle == 0u);

    NOX_DECLARE_PLUGIN_FUNCTION(AllocateRendererFunction, Renderer *, const RendererDescriptor &);
    const auto &plugin = pluginManager[pluginHandle];
    AllocateRendererFunction allocateRenderer = plugin[RendererPlugin::allocateRendererProcedureName];
    auto *renderer = allocateRenderer(descriptor);
    auto deleter = [&pluginManager, pluginHandle](Renderer *renderer) {
        delete renderer;
        pluginManager.unregisterPlugin(pluginHandle);
    };

    return {renderer, deleter};
}

} // namespace NOX
