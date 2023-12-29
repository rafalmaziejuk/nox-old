#pragma once

#include <nox/export.h>
#include <nox/format.h>
#include <nox/vector.h>

#include <variant>
#include <vector>

namespace nox {

class Framebuffer;
class RenderPass;
class RenderPassVisitor;

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

struct ClearDepthStencilValue {
    float depth;
    uint32_t stencil;
};

using ClearColorValue = std::variant<Vector4D<float>, Vector4D<int32_t>, Vector4D<uint32_t>>;
using ClearValue = std::variant<ClearColorValue, ClearDepthStencilValue>;
using ClearValues = std::vector<ClearValue>;

struct RenderPassBeginDescriptor {
    const RenderPass *renderPass;
    const Framebuffer *framebuffer;
    ClearValues clearValues;
};

struct RenderPassDescriptor {
    AttachmentsDescriptorsContainer attachmentsDescriptors;
};

class NOX_EXPORT RenderPass {
  public:
    virtual void accept(RenderPassVisitor &visitor) const = 0;

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
