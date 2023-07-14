#pragma once

#include <nox/format.h>

#include <array>
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

} // namespace NOX
