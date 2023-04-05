#include "plugins/plugin.h"
#include "plugins/plugin_loader.h"
#include "plugins/plugin_procedure_names.h"

#include <nox/renderer/renderer.h>

namespace NOX {

struct Renderer::Impl {
    const Plugin *rendererPlugin{nullptr};
};

Renderer::Renderer() : m_impl{std::make_unique<Impl>()} {}

Renderer::~Renderer() = default;

std::unique_ptr<Renderer, RendererDeleter> Renderer::create(std::string_view rendererName) {
    NOX_DECLARE_PLUGIN_FUNCTION(AllocateRendererFunction, void *, void);
    NOX_DECLARE_PLUGIN_FUNCTION(DeallocateRendererFunction, void, void *);

    const auto *plugin = PluginLoader::instance().loadPlugin(rendererName);
    auto *allocateProcedure = plugin->loadProcedure(RendererPlugin::allocateRendererProcedureName);
    auto *allocateRendererFunction = reinterpret_cast<AllocateRendererFunction>(allocateProcedure);
    auto *deallocateProcedure = plugin->loadProcedure(RendererPlugin::deallocateRendererProcedureName);
    auto *deallocateRendererFunction = reinterpret_cast<DeallocateRendererFunction>(deallocateProcedure);

    auto *renderer = reinterpret_cast<Renderer *>(allocateRendererFunction());
    renderer->m_impl->rendererPlugin = plugin;

    return {renderer, RendererDeleter{deallocateRendererFunction}};
}

void Renderer::unload(std::unique_ptr<Renderer, RendererDeleter> &renderer) {
    const auto *plugin = renderer->m_impl->rendererPlugin;
    renderer.reset();

    PluginLoader::instance().unloadPlugin(plugin);
}

} // namespace NOX
