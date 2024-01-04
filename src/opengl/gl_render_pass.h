#pragma once

#include <nox/render_pass.h>

namespace nox {

class GLRenderPass final : public RenderPass {
  public:
    [[nodiscard]] static bool validateInput(const RenderPassDescriptor &descriptor);

    explicit GLRenderPass(const RenderPassDescriptor &descriptor);

    const AttachmentsDescriptorsContainer &getAttachmentsDescriptors() const {
        return m_attachmentsDescriptors;
    }

  private:
    AttachmentsDescriptorsContainer m_attachmentsDescriptors{};
};

} // namespace nox
