#pragma once

#include <nox/export.h>
#include <nox/renderer.h>

namespace nox {

using CreateRendererCallback = std::function<Renderer *()>;
using DestroyRendererCallback = std::function<void(Renderer *)>;
using RendererFactory = std::pair<CreateRendererCallback, DestroyRendererCallback>;

class NOX_EXPORT RendererFactoryRegistry {
  public:
    [[nodiscard]] static bool loadRendererPlugin(RendererBackend backend);
    [[nodiscard]] static RendererPtr createRenderer();

    static void initializeFactory(RendererBackend backend, const RendererFactory &factory);
};

} // namespace nox
