#pragma once

#include <nox/renderer.h>

namespace NOX {

class Plugin;

class RendererPlugin {
  public:
    static constexpr auto allocateRendererProcedureName = "allocateRenderer";

    [[nodiscard]] static const char *getRendererName(RendererAPI api);
    [[nodiscard]] static std::unique_ptr<Plugin> load(RendererAPI api, std::string_view pluginName);
};

} // namespace NOX
