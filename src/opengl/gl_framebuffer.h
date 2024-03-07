#pragma once

#include <nox/command_list.h>
#include <nox/framebuffer.h>
#include <nox/render_pass.h>

namespace nox {

using AttachmentPoint = uint32_t;
using AttachmentPoints = std::vector<AttachmentPoint>;

class GLFramebuffer final : public Framebuffer {
  public:
    [[nodiscard]] static bool validateInput(const FramebufferDescriptor &descriptor);

    explicit GLFramebuffer(const FramebufferDescriptor &descriptor);
    ~GLFramebuffer() override;

    [[nodiscard]] bool validate() const;
    void bind() const;
    void unbind() const;

    void clearAttachments(const ClearValues &values, const AttachmentDescriptors &attachmentDescriptors) const;
    void bindColorAttachments(const ColorAttachmentReferences &attachmentReferences) const;

    void invalidateAttachments(const AttachmentDescriptors &attachmentDescriptors) const;

  private:
    void attachColorAttachments(const Attachments &attachments);
    void attachDepthStencilAttachments(const Attachments &attachments);
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
