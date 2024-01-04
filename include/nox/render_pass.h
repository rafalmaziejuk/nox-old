#pragma once

#include <nox/export.h>
#include <nox/format.h>

#include <vector>

namespace nox {

enum class AttachmentLoadOp {
    LOAD,
    CLEAR,
    DONT_CARE
};

enum class AttachmentStoreOp {
    STORE,
    DONT_CARE
};

struct AttachmentDescriptor {
    ImageFormat format;
    AttachmentLoadOp loadOp;
    AttachmentStoreOp storeOp;
    AttachmentLoadOp stencilLoadOp;
    AttachmentStoreOp stencilStoreOp;
};
using AttachmentsDescriptorsContainer = std::vector<AttachmentDescriptor>;

struct RenderPassDescriptor {
    AttachmentsDescriptorsContainer attachmentsDescriptors;
};

class NOX_EXPORT RenderPass {
  public:
    RenderPass(const RenderPass &) = delete;
    RenderPass &operator=(const RenderPass &) = delete;
    RenderPass(RenderPass &&) = delete;
    RenderPass &operator=(RenderPass &&) = delete;
    virtual ~RenderPass() = default;

  protected:
    RenderPass() = default;
};

} // namespace nox
