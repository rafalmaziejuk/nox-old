#pragma once

#include <nox/renderer_types.h>

#include <array>

namespace NOX::RendererPlugin {

inline constexpr std::array<std::pair<RendererAPI, const char *>, static_cast<size_t>(RendererAPI::MAX)> rendererPluginNames{{
    {RendererAPI::OPENGL, "opengl"},
}};

inline const char *getRendererPluginName(RendererAPI api) {
    return (rendererPluginNames.at(static_cast<size_t>(api)).second);
}

inline constexpr auto allocateRendererProcedureName{"allocateRenderer"};

} // namespace NOX::RendererPlugin
