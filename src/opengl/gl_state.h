#pragma once

namespace nox {

class GLFramebuffer;
class GLRenderPass;

struct GLState {
    const GLFramebuffer *currentFramebuffer{nullptr};
    const GLRenderPass *currentRenderPass{nullptr};

    uint32_t indexType;
    uint32_t primitiveTopology;
};

} // namespace nox
