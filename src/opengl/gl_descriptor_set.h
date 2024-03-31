#pragma once

#include <nox/descriptor_set.h>

namespace nox {

class GLTexture;

class GLResourceBinding {
  public:
    explicit GLResourceBinding(uint32_t index) : m_bindingIndex{index} {}
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
    std::vector<const GLTexture *> m_textures;
};

class GLInputAttachmentBinding final : public GLTextureBinding {
  public:
    using GLTextureBinding::GLTextureBinding;
};

class GLDescriptorSet final : public DescriptorSet {
  public:
    explicit GLDescriptorSet(const DescriptorSetLayout &setLayout);

    void bind() const;

  private:
    std::vector<GLResourceBinding> m_bindings;
};

} // namespace nox
