#include "plugins/plugin.h"
#include "plugins/plugin_loader.h"
#include "renderer/renderer_plugin.h"

#include <nox/renderer.h>

namespace NOX {

Renderer::~Renderer() = default;

std::unique_ptr<Renderer, RendererDeleter> Renderer::create(const RendererDescriptor &descriptor) {
    const auto *pluginName = RendererPlugin::getRendererPluginName(descriptor.api);
    const auto *plugin = PluginLoader::instance().load(pluginName);

    NOX_DECLARE_PLUGIN_FUNCTION(AllocateRendererFunction, Renderer *, const RendererDescriptor &);
    AllocateRendererFunction allocateRenderer = plugin->getFunction(RendererPlugin::allocateRendererProcedureName);
    auto *renderer = allocateRenderer(descriptor);
    auto deleter = [plugin](Renderer *renderer) {
        delete renderer;
        PluginLoader::instance().unload(plugin);
    };

    return {renderer, deleter};
}

} // namespace NOX
