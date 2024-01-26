#pragma once

#include <nox/export.h>
#include <nox/render_pass.h>
#include <nox/texture.h>
#include <nox/vector.h>

#include <cstdint>
#include <vector>

namespace nox {

using Attachments = std::vector<const Texture *>;

struct FramebufferDescriptor {
    Attachments attachments;
    const RenderPass *renderPass;
    Vector2D<uint32_t> size;
};

class NOX_EXPORT Framebuffer {
  public:
    Framebuffer(const Framebuffer &) = delete;
    Framebuffer &operator=(const Framebuffer &) = delete;
    Framebuffer(Framebuffer &&) = delete;
    Framebuffer &operator=(Framebuffer &&) = delete;
    virtual ~Framebuffer() = default;

  protected:
    Framebuffer() = default;
};

} // namespace nox
