#pragma once

#include "core/singleton.h"

#include <nox/export.h>
#include <nox/renderer.h>

#include <functional>
#include <unordered_map>

namespace NOX {

class NOX_EXPORT RendererCallbackRegistry : public Singleton<RendererCallbackRegistry> {
  public:
    using CreateCallback = std::function<Renderer *()>;
    using DestroyCallback = std::function<void(Renderer *)>;
    using Callback = std::pair<CreateCallback, DestroyCallback>;

    void registerCallback(RendererBackend backend, const Callback &callback);
    [[nodiscard]] Callback getCallback(RendererBackend backend);

  private:
    std::unordered_map<RendererBackend, Callback> m_callbacks;
};

} // namespace NOX
