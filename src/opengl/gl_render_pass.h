#pragma once

#include <nox/render_pass.h>

namespace nox {

class GLRenderPass final : public RenderPass {
  public:
    [[nodiscard]] static bool validateInput(const RenderPassDescriptor &descriptor);
    [[nodiscard]] static bool validateInput(const RenderPassBeginDescriptor &descriptor);

    explicit GLRenderPass(const RenderPassDescriptor &descriptor);

    const AttachmentsDescriptorsContainer &getAttachmentsDescriptors() const {
        return m_attachmentsDescriptors;
    }

    void accept(RenderPassVisitor &visitor) const override;

  private:
    AttachmentsDescriptorsContainer m_attachmentsDescriptors{};
};

} // namespace nox
