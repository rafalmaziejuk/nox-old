#pragma once

#include <nox/export.h>
#include <nox/format.h>

#include <cstdint>
#include <vector>

namespace nox {

enum class AttachmentLoadOp {
    NONE,
    LOAD,
    CLEAR,
    DONT_CARE,
    MAX
};

enum class AttachmentStoreOp {
    NONE,
    STORE,
    DONT_CARE,
    MAX
};

struct AttachmentDescriptor {
    ImageFormat format;
    AttachmentLoadOp loadOp;
    AttachmentStoreOp storeOp;
};
using AttachmentDescriptors = std::vector<AttachmentDescriptor>;

struct AttachmentReference {
    static constexpr auto attachmentUnused = ~0u;

    uint32_t index{attachmentUnused};
};
using InputAttachmentReferences = std::vector<AttachmentReference>;
using ColorAttachmentReferences = std::vector<AttachmentReference>;
using DepthStencilAttachmentReference = AttachmentReference;

struct SubpassDescriptor {
    InputAttachmentReferences inputAttachmentReferences;
    ColorAttachmentReferences colorAttachmentReferences;
    DepthStencilAttachmentReference depthStencilAttachmentReference;
};
using SubpassDescriptors = std::vector<SubpassDescriptor>;

struct RenderPassDescriptor {
    AttachmentDescriptors attachmentDescriptors;
    SubpassDescriptors subpassDescriptors;
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
