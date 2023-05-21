#pragma once

#include <nox/renderer/format.h>

#include <array>
#include <memory>

namespace NOX {

class Texture;

struct ColorAttachment {
    std::shared_ptr<Texture> texture;
};

struct DepthStencilAttachment {
    Format format;
};

struct RenderTargetAttachmentsContainer {
    static constexpr auto maxColorAttachments = 8u;
    static constexpr auto maxDepthStencilAttachments = 2u;
    using ColorAttachmentsContainer = std::array<ColorAttachment, maxColorAttachments>;
    using DepthStencilAttachmentsContainer = std::array<DepthStencilAttachment, maxDepthStencilAttachments>;

    ColorAttachmentsContainer colorAttachments;
    DepthStencilAttachmentsContainer depthStencilAttachments;
};

} // namespace NOX
