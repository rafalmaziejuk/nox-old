#pragma once

#include <nox/pipeline_layout.h>

namespace nox {

class GLTexture;

class GLResourceBinding {
  public:
    explicit GLResourceBinding(uint32_t index)
        : m_bindingIndex{index} {}

    virtual ~GLResourceBinding() = default;

    virtual void bind() const = 0;

  protected:
    uint32_t m_bindingIndex;
};

class GLTextureBinding : public GLResourceBinding {
  public:
    GLTextureBinding(const DescriptorSetLayoutBinding &binding);

    void bind() const override;

  protected:
    std::vector<std::shared_ptr<GLTexture>> m_textures;
};

class GLInputAttachmentBinding final : public GLTextureBinding {
  public:
    using GLTextureBinding::GLTextureBinding;
};

using GLTextureBindings = std::vector<GLTextureBinding>;
using GLInputAttachmentBindings = std::vector<GLInputAttachmentBinding>;

class GLPipelineLayout final {
  public:
    [[nodiscard]] static bool validateInput(const PipelineLayoutDescriptor &descriptor);

    explicit GLPipelineLayout(const PipelineLayoutDescriptor &descriptor);

    [[nodiscard]] const GLTextureBindings &getTextureBindings() const {
        return m_textureBindings;
    }

    [[nodiscard]] const GLInputAttachmentBindings &getInputAttachmentBindings() const {
        return m_inputAttachmentBindings;
    }

  private:
    GLTextureBindings m_textureBindings;
    GLInputAttachmentBindings m_inputAttachmentBindings;
};

} // namespace nox
