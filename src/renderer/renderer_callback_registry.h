#pragma once

#include "core/singleton.h"

#include <nox/export.h>
#include <nox/renderer.h>

#include <functional>

namespace NOX {

class NOX_EXPORT RendererCallbackRegistry : public Singleton<RendererCallbackRegistry> {
  public:
    RendererCallbackRegistry();
    ~RendererCallbackRegistry();

    using CreateCallback = std::function<Renderer *()>;
    using DestroyCallback = std::function<void(Renderer *)>;
    using Callback = std::pair<CreateCallback, DestroyCallback>;

    void registerCallback(RendererBackend backend, const Callback &callback);
    [[nodiscard]] Callback getCallback(RendererBackend backend);

  private:
    struct Impl;
    Impl *m_impl{nullptr};
};

} // namespace NOX
