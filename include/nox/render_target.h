#pragma once

#include <nox/common.h>
#include <nox/export.h>
#include <nox/texture.h>
#include <nox/vector.h>

#include <array>
#include <cstdint>
#include <memory>

namespace NOX {

class Texture;

struct AttachmentsContainer {
    static constexpr auto maxColorAttachments = 8u;
    static constexpr auto maxDepthStencilAttachments = 2u;
    using ColorAttachmentsContainer = std::array<std::shared_ptr<Texture>, maxColorAttachments>;
    using DepthStencilAttachmentsContainer = std::array<Format, maxDepthStencilAttachments>;

    ColorAttachmentsContainer colorAttachments;
    DepthStencilAttachmentsContainer depthStencilAttachments;
};

struct RenderTargetDescriptor {
    AttachmentsContainer attachments;
    Vector2D<uint32_t> size;
};

class NOX_EXPORT RenderTarget {
  public:
    RenderTarget(const RenderTarget &) = delete;
    RenderTarget &operator=(const RenderTarget &) = delete;
    RenderTarget(RenderTarget &&) = delete;
    RenderTarget &operator=(RenderTarget &&) = delete;
    virtual ~RenderTarget() = default;

  protected:
    RenderTarget() = default;
};

} // namespace NOX
