#pragma once

#include <nox/command_list.h>
#include <nox/framebuffer.h>
#include <nox/render_pass.h>

namespace nox {

class GLRenderPass;

using AttachmentPoint = uint32_t;
using AttachmentPoints = std::vector<AttachmentPoint>;

class GLFramebuffer final : public Framebuffer {
  public:
    [[nodiscard]] static std::unique_ptr<GLFramebuffer> create(const FramebufferDescriptor &descriptor);

    GLFramebuffer();
    explicit GLFramebuffer(const FramebufferDescriptor &descriptor);
    ~GLFramebuffer() override;

    void bind() const;
    void clearAttachments(const ClearValues &values, const AttachmentDescriptors &descriptors) const;
    void invalidateAttachments(const AttachmentDescriptors &descriptors) const;
    void bindColorAttachments(const ColorAttachmentReferences &references) const;

    [[nodiscard]] bool isComplete() const;
    [[nodiscard]] bool isCompatible(const GLRenderPass *renderPass) const;
    [[nodiscard]] bool attachAttachments(const Attachments &attachments);

  private:
    void attachAttachment(const Texture *attachment, AttachmentPoint attachmentPoint);

    void clearColorAttachment(const ClearColorValue *value, int32_t index) const;
    void clearDepthAttachment(const ClearDepthStencilValue *value) const;
    void clearStencilAttachment(const ClearDepthStencilValue *value) const;
    void clearDepthStencilAttachment(const ClearDepthStencilValue *value) const;

  private:
    AttachmentPoints m_attachmentPoints;
    uint32_t m_handle{0u};
};

} // namespace nox
