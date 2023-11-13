#pragma once

#include <nox/buffer.h>
#include <nox/common.h>
#include <nox/command_list.h>
#include <nox/export.h>
#include <nox/graphics_pipeline_state.h>
#include <nox/render_target.h>
#include <nox/shader.h>
#include <nox/swap_chain.h>
#include <nox/texture.h>
#include <nox/window.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>

namespace NOX {

enum class RendererBackend : uint8_t {
    OPENGL = 0u,
    MAX
};

class Renderer;
using RendererDeleter = std::function<void(Renderer *)>;
using RendererPtr = std::unique_ptr<Renderer, RendererDeleter>;

class NOX_EXPORT Renderer {
  public:
    [[nodiscard]] static RendererPtr create(RendererBackend backend);

    [[nodiscard]] virtual RendererBackend getRendererBackend() const = 0;

    [[nodiscard]] virtual std::unique_ptr<SwapChain> createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) = 0;

    [[nodiscard]] virtual std::unique_ptr<Buffer> createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &vertexFormat) = 0;

    [[nodiscard]] virtual std::unique_ptr<Buffer> createIndexBuffer(const BufferDescriptor &descriptor, Format format) = 0;

    [[nodiscard]] virtual std::unique_ptr<Shader> createShader(const ShaderDescriptor &descriptor, std::string_view source) = 0;

    [[nodiscard]] virtual std::unique_ptr<GraphicsPipelineState> createGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor) = 0;

    [[nodiscard]] virtual std::unique_ptr<CommandList> createCommandList(const CommandListDescriptor &descriptor) = 0;

    [[nodiscard]] virtual std::unique_ptr<Texture> createTexture(const TextureDescriptor &descriptor) = 0;

    [[nodiscard]] virtual std::unique_ptr<RenderTarget> createRenderTarget(const RenderTargetDescriptor &descriptor) = 0;

  public:
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &&) = delete;
    virtual ~Renderer() = default;

  protected:
    Renderer() = default;
};

} // namespace NOX
