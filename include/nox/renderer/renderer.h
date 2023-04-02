#pragma once

#include <nox/export.h>

#include <memory>
#include <string_view>

namespace NOX {

class RendererDeleter;

class NOX_EXPORT Renderer {
  public:
    virtual ~Renderer() = default;

    [[nodiscard]] static std::unique_ptr<Renderer, RendererDeleter> create(std::string_view rendererName);
};

class RendererDeleter {
  public:
    using RendererDeallocateFunction = void (*)(void *);

  public:
    RendererDeleter() = default;
    RendererDeleter(RendererDeallocateFunction deallocateFunction) : m_deallocateFunction{deallocateFunction} {}
    void operator()(Renderer *renderer) {
        if (m_deallocateFunction != nullptr) {
            m_deallocateFunction(renderer);
        } else {
            delete renderer;
        }
    }

  private:
    RendererDeallocateFunction m_deallocateFunction{nullptr};
};

} // namespace NOX
