#pragma once

#include <nox/render_pass.h>

namespace nox {

class GLRenderPass final : public RenderPass {
  public:
    [[nodiscard]] static bool validateInput(const RenderPassDescription &description);

    explicit GLRenderPass(const RenderPassDescription &description);

    const AttachmentsDescriptionsContainer &getAttachmentsDescriptions() const {
        return m_attachmentsDescriptions;
    }

  private:
    AttachmentsDescriptionsContainer m_attachmentsDescriptions{};
};

} // namespace nox
