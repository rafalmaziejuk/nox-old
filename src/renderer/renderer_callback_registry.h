#pragma once

#include <nox/export.h>
#include <nox/renderer.h>

#include <functional>
#include <unordered_map>

namespace NOX {

class NOX_EXPORT RendererCallbackRegistry {
  public:
    using CreateCallback = std::function<Renderer *()>;
    using DestroyCallback = std::function<void(Renderer *)>;
    using Callback = std::pair<CreateCallback, DestroyCallback>;

    static void registerCallback(RendererBackend backend, const Callback &callback);
    [[nodiscard]] static bool isCallbackRegistered(RendererBackend backend);
    [[nodiscard]] static Callback getCallback(RendererBackend backend);

  private:
    static std::unordered_map<RendererBackend, Callback> m_callbacks;
};

} // namespace NOX
