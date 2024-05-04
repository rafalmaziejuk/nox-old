#pragma once

#include <nox/export.h>
#include <nox/renderer.h>

#include <optional>

namespace nox {

using CreateRendererCallback = std::function<Renderer *()>;
using DestroyRendererCallback = std::function<void(Renderer *)>;
using RendererFactory = std::pair<CreateRendererCallback, DestroyRendererCallback>;

class NOX_EXPORT RendererFactoryRegistry {
  public:
    [[nodiscard]] static RendererFactoryRegistry &instance();

    [[nodiscard]] bool initialize(RendererBackend backend);

    void registerFactory(RendererBackend backend, const RendererFactory &factory);
    [[nodiscard]] const std::optional<RendererFactory> &getFactory(RendererBackend backend) const;

  public:
    RendererFactoryRegistry(const RendererFactoryRegistry &) = delete;
    RendererFactoryRegistry &operator=(const RendererFactoryRegistry &) = delete;
    RendererFactoryRegistry(RendererFactoryRegistry &&) = delete;
    RendererFactoryRegistry &operator=(RendererFactoryRegistry &&) = delete;

  private:
    RendererFactoryRegistry();
    ~RendererFactoryRegistry();

  private:
    struct Impl;
    Impl *m_impl{nullptr};
};

} // namespace nox
