#pragma once

#include <base/example.h>

namespace nox {

class SandboxExample : public base::Example {
  public:
    void initialize() override;
    void onUpdate() override;

  private:
    std::unique_ptr<CommandList> m_commandList{nullptr};
    std::unique_ptr<RenderPass> m_renderPass{nullptr};
    std::unique_ptr<Framebuffer> m_framebuffer{nullptr};
    std::unique_ptr<GraphicsPipelineState> m_graphicsPipelineState{nullptr};
    std::unique_ptr<Buffer> m_vertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_indexBuffer{nullptr};
};

} // namespace nox
