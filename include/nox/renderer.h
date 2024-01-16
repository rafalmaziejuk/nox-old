#pragma once

#include <nox/buffer.h>
#include <nox/command_list.h>
#include <nox/framebuffer.h>
#include <nox/export.h>
#include <nox/graphics_pipeline_state.h>
#include <nox/pipeline_layout.h>
#include <nox/render_pass.h>
#include <nox/shader.h>
#include <nox/swapchain.h>
#include <nox/texture.h>

#include <functional>
#include <memory>
#include <string_view>

namespace nox {

enum class RendererBackend {
    NONE,
    OPENGL
};

class Renderer;
using RendererDeleter = std::function<void(Renderer *)>;
using RendererPtr = std::unique_ptr<Renderer, RendererDeleter>;

class NOX_EXPORT Renderer {
  public:
    [[nodiscard]] static RendererPtr create(RendererBackend backend);

    [[nodiscard]] virtual RendererBackend getRendererBackend() const = 0;

    [[nodiscard]] virtual std::unique_ptr<Swapchain> createSwapchain(const SwapchainDescription &description) = 0;

    [[nodiscard]] virtual std::unique_ptr<Buffer> createVertexBuffer(const VertexBufferDescription &description) = 0;

    [[nodiscard]] virtual std::unique_ptr<Buffer> createIndexBuffer(const IndexBufferDescription &description) = 0;

    [[nodiscard]] virtual std::unique_ptr<Shader> createShader(const ShaderDescription &description, std::string_view source) = 0;

    [[nodiscard]] virtual std::unique_ptr<PipelineLayout> createPipelineLayout(PipelineLayoutDescription &description) = 0;

    [[nodiscard]] virtual std::unique_ptr<GraphicsPipelineState> createGraphicsPipelineState(GraphicsPipelineStateDescription &description) = 0;

    [[nodiscard]] virtual std::unique_ptr<CommandList> createCommandList(const CommandListDescription &description) = 0;

    [[nodiscard]] virtual std::shared_ptr<Texture> createTexture2D(const Texture2DDescription &description) = 0;

    [[nodiscard]] virtual std::unique_ptr<RenderPass> createRenderPass(const RenderPassDescription &description) = 0;

    [[nodiscard]] virtual std::unique_ptr<Framebuffer> createFramebuffer(const FramebufferDescription &description) = 0;

  public:
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &&) = delete;
    virtual ~Renderer() = default;

  protected:
    Renderer() = default;
};

} // namespace nox
