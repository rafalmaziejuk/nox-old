#pragma once

#include <nox/export.h>
#include <nox/vec.h>
#include <nox/render_target_types.h>

namespace NOX {

struct RenderTargetDescriptor {
    AttachmentsContainer attachments;
    Vector2D<uint32_t> size;
};

class NOX_EXPORT RenderTarget {
  public:
    RenderTarget(const RenderTarget &) = delete;
    RenderTarget &operator=(const RenderTarget &) = delete;
    virtual ~RenderTarget();

  protected:
    RenderTarget() = default;
};

} // namespace NOX
