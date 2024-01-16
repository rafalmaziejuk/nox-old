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

struct AttachmentDescription {
    ImageFormat format;
    AttachmentLoadOp loadOp;
    AttachmentStoreOp storeOp;
};
using AttachmentsDescriptionsContainer = std::vector<AttachmentDescription>;

struct RenderPassDescription {
    AttachmentsDescriptionsContainer attachmentsDescriptions;
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
