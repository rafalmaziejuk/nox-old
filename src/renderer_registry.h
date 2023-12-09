#pragma once

#include "plugins/plugin.h"

#include <nox/export.h>
#include <nox/renderer.h>

namespace NOX {

using RendererFactory = std::pair<std::function<Renderer *()>,
                                  std::function<void(Renderer *)>>;

class NOX_EXPORT RendererRegistry {
  public:
    static RendererRegistry &instance();

    [[nodiscard]] bool loadPlugin(RendererBackend backend);

    void registerFactory(RendererBackend backend, const RendererFactory &factory);

    [[nodiscard]] RendererPtr createRenderer(RendererBackend backend);
    [[nodiscard]] bool releaseRenderer(RendererPtr &&renderer);

  private:
    [[nodiscard]] bool containsFactory(RendererBackend backend) const;

  public:
    RendererRegistry(const RendererRegistry &) = delete;
    RendererRegistry &operator=(const RendererRegistry &) = delete;
    RendererRegistry(RendererRegistry &&) = delete;
    RendererRegistry &operator=(RendererRegistry &&) = delete;

  private:
    RendererRegistry() = default;

  private:
    static constexpr auto registrySize = static_cast<size_t>(RendererBackend::MAX);
    using RendererPlugin = std::unique_ptr<Plugin>;
    using RendererContainer = std::vector<RendererPtr>;

    std::array<RendererFactory, registrySize> m_factories{};
    std::array<RendererPlugin, registrySize> m_plugins{};
    std::array<RendererContainer, registrySize> m_renderersContainers{};
};

} // namespace NOX
