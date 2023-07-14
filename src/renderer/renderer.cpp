#include "plugins/plugin.h"
#include "plugins/plugin_loader.h"
#include "plugins/plugin_procedure_names.h"

#include <nox/renderer.h>

namespace NOX {

namespace {

constexpr std::array<std::pair<RendererAPI, const char *>, static_cast<size_t>(RendererAPI::MAX)> rendererPluginNames{{
    {RendererAPI::OPENGL, "OpenGL"},
}};

} // namespace

Renderer::~Renderer() = default;

std::unique_ptr<Renderer, RendererDeleter> Renderer::create(const RendererDescriptor &descriptor) {
    NOX_DECLARE_PLUGIN_FUNCTION(AllocateRendererFunction, void *, const RendererDescriptor &);
    NOX_DECLARE_PLUGIN_FUNCTION(DeallocateRendererFunction, void, void *);

    const auto *rendererPluginName = rendererPluginNames.at(static_cast<size_t>(descriptor.api)).second;
    const auto *plugin = PluginLoader::instance().loadPlugin(rendererPluginName);
    auto *allocateProcedure = plugin->loadProcedure(RendererPlugin::allocateRendererProcedureName);
    auto *allocateRendererFunction = reinterpret_cast<AllocateRendererFunction>(allocateProcedure);
    auto *deallocateProcedure = plugin->loadProcedure(RendererPlugin::deallocateRendererProcedureName);
    auto *deallocateRendererFunction = reinterpret_cast<DeallocateRendererFunction>(deallocateProcedure);

    auto *renderer = reinterpret_cast<Renderer *>(allocateRendererFunction(descriptor));
    auto deleter = [deallocateRendererFunction, plugin](Renderer *renderer) {
        deallocateRendererFunction(renderer);
        PluginLoader::instance().unloadPlugin(plugin);
    };

    return {renderer, deleter};
}

} // namespace NOX
