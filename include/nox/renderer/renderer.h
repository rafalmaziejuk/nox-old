#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/renderer/renderer_types.h>

#include <functional>
#include <memory>

namespace NOX {

class SwapChain;
struct SwapChainDescriptor;

class Window;

class Renderer;
using RendererDeleter = std::function<void(Renderer *)>;

struct RendererDescriptor {
    RendererAPI api;
    RendererConfig config;
};

class NOX_EXPORT Renderer : public NonCopyable {
  public:
    virtual ~Renderer();

    [[nodiscard]] static std::unique_ptr<Renderer, RendererDeleter> create(const RendererDescriptor &descriptor);

    [[nodiscard]] virtual std::unique_ptr<SwapChain> createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) = 0;
};

} // namespace NOX
