#pragma once

#include <nox/pipeline_layout.h>

namespace nox {

class GLTexture;

class GLResourceBinding {
  public:
    explicit GLResourceBinding(uint32_t index) : m_bindingIndex{index} {}
    virtual ~GLResourceBinding() = default;

    [[nodiscard]] virtual ResourceType getResourceType() = 0;
    [[nodiscard]] virtual std::vector<uint32_t> getHandles() const = 0;

    virtual void bind() const = 0;

  public:
    GLResourceBinding(const GLResourceBinding &) = delete;
    GLResourceBinding &operator=(const GLResourceBinding &) = delete;

  protected:
    uint32_t m_bindingIndex;
};

class GLTextureResourceBinding : public GLResourceBinding {
  public:
    GLTextureResourceBinding(const DescriptorSetLayoutBinding &binding);
    ~GLTextureResourceBinding() override = default;

    [[nodiscard]] ResourceType getResourceType() override {
        return ResourceType::TEXTURE;
    }

    std::vector<uint32_t> getHandles() const override;

    void bind() const override;

  private:
    std::vector<std::shared_ptr<GLTexture>> m_textures;
};

class GLInputAttachmentResourceBinding final : public GLTextureResourceBinding {
  public:
    using GLTextureResourceBinding::GLTextureResourceBinding;
    ~GLInputAttachmentResourceBinding() override = default;

    [[nodiscard]] ResourceType getResourceType() override {
        return ResourceType::INPUT_ATTACHMENT;
    }
};

using GLResourceBindings = std::vector<std::unique_ptr<GLResourceBinding>>;

class GLPipelineLayout final {
  public:
    [[nodiscard]] static bool validateInput(const PipelineLayoutDescriptor &descriptor);

    explicit GLPipelineLayout(const PipelineLayoutDescriptor &descriptor);

    GLResourceBindings::iterator begin() { return m_bindings.begin(); }
    GLResourceBindings::iterator end() { return m_bindings.end(); }
    GLResourceBindings::const_iterator begin() const { return m_bindings.begin(); }
    GLResourceBindings::const_iterator end() const { return m_bindings.end(); }

    [[nodiscard]] bool contains(uint32_t handle) const;

  private:
    GLResourceBindings m_bindings;
};

} // namespace nox
