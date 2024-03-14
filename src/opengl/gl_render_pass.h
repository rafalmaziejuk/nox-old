#pragma once

#include <nox/render_pass.h>

namespace nox {

class GLRenderPass final : public RenderPass {
  public:
    [[nodiscard]] static bool validateInput(const RenderPassDescriptor &descriptor);

    explicit GLRenderPass(const RenderPassDescriptor &descriptor);

    [[nodiscard]] const AttachmentDescriptors &getAttachmentDescriptors() const {
        return m_attachmentDescriptors;
    }

    [[nodiscard]] const SubpassDescriptor &getSubpassDescriptor(uint32_t index) const {
        return m_subpassDescriptors.at(index);
    }

  private:
    AttachmentDescriptors m_attachmentDescriptors;
    SubpassDescriptors m_subpassDescriptors;
};

} // namespace nox
