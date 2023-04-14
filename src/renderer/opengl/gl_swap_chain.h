#pragma once

#include <nox/renderer/swap_chain.h>

#include <memory>

namespace NOX {

class GLContext;

class GLSwapChain final : public SwapChain {
  public:
    GLSwapChain(const SwapChainDescriptor &descriptor, std::shared_ptr<GLContext> &context);

    void swap() const override;

    void setVSync(bool value) override;

  private:
    std::shared_ptr<GLContext> m_context{nullptr};
};

} // namespace NOX
