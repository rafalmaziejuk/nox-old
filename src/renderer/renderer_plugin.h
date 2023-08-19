#pragma once

#include <nox/renderer_types.h>

namespace NOX {

class Plugin;

class RendererPlugin {
  public:
    static constexpr auto allocateRendererProcedureName = "allocateRenderer";

    [[nodiscard]] static const char *getRendererName(RendererAPI api);
    [[nodiscard]] static Plugin *load(RendererAPI api, std::string_view pluginName);
};

} // namespace NOX
