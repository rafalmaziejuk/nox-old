#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/renderer/renderer_types.h>

#include <functional>
#include <memory>
#include <string_view>

namespace NOX {

class Buffer;
struct BufferDescriptor;
enum class Format : uint8_t;
struct VertexFormat;

class CommandList;
struct CommandListDescriptor;

class PipelineState;
struct PipelineStateDescriptor;

class RenderPass;
struct RenderPassDescriptor;

class RenderTarget;
struct RenderTargetDescriptor;

class Shader;
struct ShaderDescriptor;

class SwapChain;
struct SwapChainDescriptor;

class Texture;
struct TextureDescriptor;

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

    [[nodiscard]] virtual std::unique_ptr<Buffer> createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &format) = 0;

    [[nodiscard]] virtual std::unique_ptr<Buffer> createIndexBuffer(const BufferDescriptor &descriptor, Format format) = 0;

    [[nodiscard]] virtual std::unique_ptr<Shader> createShaderFromString(const ShaderDescriptor &descriptor, std::string_view source) = 0;

    [[nodiscard]] virtual std::unique_ptr<PipelineState> createPipelineState(const PipelineStateDescriptor &descriptor) = 0;

    [[nodiscard]] virtual std::unique_ptr<CommandList> createCommandList(const CommandListDescriptor &descriptor) = 0;

    [[nodiscard]] virtual std::unique_ptr<Texture> createTexture(const TextureDescriptor &descriptor) = 0;

    [[nodiscard]] virtual std::unique_ptr<RenderTarget> createRenderTarget(const RenderTargetDescriptor &descriptor) = 0;

    [[nodiscard]] virtual std::unique_ptr<RenderPass> createRenderPass(const RenderPassDescriptor &descriptor) = 0;
};

} // namespace NOX
