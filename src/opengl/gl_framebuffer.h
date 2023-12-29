#pragma once

#include "opengl/gl_texture.h"

#include <nox/framebuffer.h>

namespace nox {

using AttachmentPoint = uint32_t;
using AttachmentsPointsContainer = std::vector<AttachmentPoint>;

class GLFramebuffer : public Framebuffer {
  public:
    [[nodiscard]] static bool validateInput(const FramebufferDescriptor &descriptor);
    [[nodiscard]] static bool isDefaultFramebuffer(const AttachmentsContainer &attachments);

  public:
    ~GLFramebuffer() override;

    [[nodiscard]] bool validate() const;

    void clearAttachments(const ClearValues &values, const RenderPass *renderPass) const;
    void bind() const;

    void accept(FramebufferVisitor &visitor) const override;

  private:
    void clearColorAttachment(const ClearColorValue &value, int32_t index) const;
    void clearDepthAttachment(float depth) const;
    void clearStencilAttachment(uint32_t stencil) const;

  protected:
    AttachmentsPointsContainer m_attachmentsPoints;
    uint32_t m_handle{0u};
};

class GLFramebufferWithAttachments final : public GLFramebuffer {
  public:
    explicit GLFramebufferWithAttachments(const FramebufferDescriptor &descriptor);

  private:
    void attachColorAttachments(const AttachmentsContainer &attachments);
    void attachDepthStencilAttachments(const AttachmentsContainer &attachments);
    void attachAttachment(const Texture *attachment, AttachmentPoint attachmentPoint);
};

class GLDefaultFramebuffer final : public GLFramebuffer {
  public:
    GLDefaultFramebuffer();
};

} // namespace nox
