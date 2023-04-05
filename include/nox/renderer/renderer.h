#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>

#include <memory>
#include <string_view>

namespace NOX {

class RendererDeleter;

class NOX_EXPORT Renderer : public NonCopyable {
  public:
    virtual ~Renderer();

    [[nodiscard]] static std::unique_ptr<Renderer, RendererDeleter> create(std::string_view rendererName);

  protected:
    Renderer();

  private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
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
