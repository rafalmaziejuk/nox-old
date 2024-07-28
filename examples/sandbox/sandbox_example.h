#pragma once

#include <base/example.h>

namespace nox {

class SandboxExample : public base::Example {
  public:
    SandboxExample();

    void initialize() override;
    void onUpdate() override;

  private:
    void createBuffer();
    void createTexture();
    void createRenderPass();
    void createFramebuffer();
    void createPipelineLayout();
    void createGraphicsPipelineState();

  private:
    std::unique_ptr<CommandList> m_commandList{nullptr};
    std::unique_ptr<Buffer> m_vertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_indexBuffer{nullptr};
    std::shared_ptr<Texture> m_texture{nullptr};
    std::unique_ptr<RenderPass> m_renderPass{nullptr};
    std::unique_ptr<Framebuffer> m_framebuffer{nullptr};
    std::shared_ptr<PipelineLayout> m_pipelineLayout{nullptr};
    std::unique_ptr<GraphicsPipelineState> m_graphicsPipelineState{nullptr};
};

} // namespace nox
