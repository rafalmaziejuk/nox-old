#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/vec.h>
#include <nox/renderer/render_target_types.h>

namespace NOX {

struct RenderTargetDescriptor {
    RenderTargetAttachmentsContainer attachments;
    Vector2D<uint32_t> resolution;
};

class NOX_EXPORT RenderTarget : public NonCopyable {
  public:
    virtual ~RenderTarget();

    // virtual void resize(const Vector2D<uint32_t> &size) = 0;
};

} // namespace NOX
