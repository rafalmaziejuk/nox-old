#pragma once

#include "plugins/plugin.h"

#include <nox/export.h>
#include <nox/renderer.h>

namespace nox {

using CreateRendererCallback = std::function<Renderer *()>;
using DestroyRendererCallback = std::function<void(Renderer *)>;
using RendererFactory = std::pair<CreateRendererCallback, DestroyRendererCallback>;

class NOX_EXPORT RendererFactoryRegistry {
  public:
    static RendererFactoryRegistry &instance();

    [[nodiscard]] bool loadPlugin(RendererBackend backend);

    [[nodiscard]] RendererPtr createRenderer(RendererBackend backend);

    void registerFactory(RendererBackend backend, const RendererFactory &factory);

  private:
    [[nodiscard]] bool contains(RendererBackend backend) const;

  public:
    RendererFactoryRegistry(const RendererFactoryRegistry &) = delete;
    RendererFactoryRegistry &operator=(const RendererFactoryRegistry &) = delete;
    RendererFactoryRegistry(RendererFactoryRegistry &&) = delete;
    RendererFactoryRegistry &operator=(RendererFactoryRegistry &&) = delete;

  private:
    RendererFactoryRegistry() = default;

  private:
    static constexpr auto registrySize = static_cast<size_t>(RendererBackend::MAX);
    using RendererPlugin = std::unique_ptr<Plugin>;

    std::array<RendererFactory, registrySize> m_factories{};
    std::array<RendererPlugin, registrySize> m_plugins{};
};

} // namespace nox