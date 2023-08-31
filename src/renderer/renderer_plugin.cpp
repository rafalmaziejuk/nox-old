#include "plugins/plugin.h"
#include "renderer/renderer_plugin.h"

#if defined(NOX_BUILD_RENDERER_OPENGL)
#include "renderer/opengl/gl_plugin.h"
#endif

namespace NOX {

namespace {

constexpr std::array<std::pair<RendererAPI, const char *>, static_cast<size_t>(RendererAPI::MAX)> rendererNames{{
    {RendererAPI::OPENGL, "nox-opengl"},
}};

} // namespace

const char *RendererPlugin::getRendererName(RendererAPI api) {
    NOX_ASSERT(api >= RendererAPI::MAX);

    const auto *pluginName = rendererNames.at(static_cast<size_t>(api)).second;
    return pluginName;
}

std::unique_ptr<Plugin> RendererPlugin::load(RendererAPI api, std::string_view pluginName) {
    NOX_ASSERT((api >= RendererAPI::MAX) || (pluginName.empty()));

    std::unique_ptr<Plugin> plugin{nullptr};
    if constexpr (Config::staticEnabled) {
#if defined(NOX_BUILD_RENDERER_OPENGL)
        if (api == RendererAPI::OPENGL) {
            plugin = std::make_unique<GLPlugin>(pluginName);
        }
#endif
    } else {
        plugin = Plugin::load(pluginName);
    }

    NOX_ASSERT(plugin == nullptr);
    return plugin;
}

} // namespace NOX
