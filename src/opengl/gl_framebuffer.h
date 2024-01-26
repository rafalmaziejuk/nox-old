#pragma once

#include "opengl/gl_texture.h"

#include <nox/command_list.h>
#include <nox/framebuffer.h>
#include <nox/render_pass.h>

namespace nox {

class GLPipelineLayout;
struct ImageFormatDescriptor;

using AttachmentPoint = uint32_t;
using AttachmentPoints = std::vector<AttachmentPoint>;

class GLFramebufferBase : public Framebuffer {
  public:
    [[nodiscard]] static bool validateInput(const FramebufferDescriptor &descriptor);
    [[nodiscard]] static bool isDefaultFramebuffer(const Attachments &attachments);

  public:
    ~GLFramebufferBase() override;

    [[nodiscard]] bool validate() const;

    void clearAttachments(const ClearValues &values, const AttachmentDescriptors &attachmentDescriptors) const;
    void invalidateAttachments(const AttachmentDescriptors &attachmentDescriptors) const;
    void bindAttachments(const SubpassDescriptor &subpassDescriptor, const GLPipelineLayout &pipelineLayout) const;

    void bind() const;

  private:
    void clearColorAttachment(const ClearColorValue *value, int32_t index) const;
    void clearDepthAttachment(const ClearDepthStencilValue *value) const;
    void clearStencilAttachment(const ClearDepthStencilValue *value) const;
    void clearDepthStencilAttachment(const ClearDepthStencilValue *value) const;

  protected:
    AttachmentPoints m_attachmentPoints;
    uint32_t m_handle{0u};
};

class GLFramebuffer final : public GLFramebufferBase {
  public:
    explicit GLFramebuffer(const FramebufferDescriptor &descriptor);

  private:
    void attachColorAttachments(const Attachments &attachments);
    void attachDepthStencilAttachments(const Attachments &attachments);
    void attachAttachment(const Texture *attachment, AttachmentPoint attachmentPoint);
};

class GLDefaultFramebuffer final : public GLFramebufferBase {
  public:
    GLDefaultFramebuffer();
};

} // namespace nox
