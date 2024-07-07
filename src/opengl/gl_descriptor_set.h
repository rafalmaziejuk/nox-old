#pragma once

#include <nox/pipeline_layout.h>

namespace nox {

class GLDescriptorSetBinding {
  public:
    GLDescriptorSetBinding(DescriptorType type, uint32_t index);
    virtual ~GLDescriptorSetBinding() = default;

    virtual void bind() const = 0;

  protected:
    DescriptorType m_type{DescriptorType::NONE};
    uint32_t m_index{0u};
};

class GLTextureBinding final : public GLDescriptorSetBinding {
  public:
    explicit GLTextureBinding(const DescriptorSetLayoutBindingDescriptor &descriptor);

    void bind() const override;

  private:
    std::vector<std::shared_ptr<Texture>> m_textures;
};

class GLDescriptorSet final {
  public:
    explicit GLDescriptorSet(const DescriptorSetLayoutDescriptor &descriptor);

    void bind() const;

  private:
    std::vector<std::unique_ptr<GLDescriptorSetBinding>> m_bindings;
};

} // namespace nox
