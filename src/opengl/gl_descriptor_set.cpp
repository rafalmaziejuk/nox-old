#include "asserts.h"
#include "opengl/gl_descriptor_set.h"
#include "opengl/gl_texture.h"

namespace nox {

GLDescriptorSet::GLDescriptorSet(const DescriptorSetLayoutDescriptor &descriptor) {
    for (const auto &bindingDescriptor : descriptor.bindingDescriptors) {
        switch (bindingDescriptor.descriptorType) {
        case DescriptorType::TEXTURE:
            m_bindings.emplace_back(std::make_unique<GLTextureBinding>(bindingDescriptor));
            break;

        default: NOX_ASSERT(false);
        }
    }
}

void GLDescriptorSet::bind() const {
    for (const auto &binding : m_bindings) {
        binding->bind();
    }
}

GLDescriptorSetBinding::GLDescriptorSetBinding(DescriptorType type, uint32_t index)
    : m_type{type},
      m_index{index} {}

GLTextureBinding::GLTextureBinding(const DescriptorSetLayoutBindingDescriptor &descriptor)
    : GLDescriptorSetBinding{descriptor.descriptorType, descriptor.bindingIndex} {
    for (const auto &textureBindingDescriptor : descriptor.textureBindingDescriptors) {
        m_textures.push_back(textureBindingDescriptor.texture);
    }
}

void GLTextureBinding::bind() const {
    uint32_t currentTextureBindingIndex = m_index;
    for (const auto &texture : m_textures) {
        const auto *glTexture = static_cast<const GLTexture *>(texture.get());
        glTexture->bind(currentTextureBindingIndex);
        currentTextureBindingIndex++;
    }
}

} // namespace nox
