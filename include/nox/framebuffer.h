#pragma once

#include <nox/export.h>
#include <nox/render_pass.h>
#include <nox/texture.h>
#include <nox/vector.h>

#include <cstdint>
#include <vector>

namespace nox {

class FramebufferVisitor;

using AttachmentsContainer = std::vector<const Texture *>;

struct FramebufferDescriptor {
    AttachmentsContainer attachments;
    const RenderPass *renderPass;
    Vector2D<uint32_t> size;
};

class NOX_EXPORT Framebuffer {
  public:
    virtual void accept(FramebufferVisitor &visitor) const = 0;

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
