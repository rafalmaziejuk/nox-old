#include "opengl/gl_pipeline_layout.h"
#include "opengl/gl_texture.h"

namespace nox {

bool GLPipelineLayout::validateInput(const PipelineLayoutDescriptor &descriptor) {
    bool result = true;

    for (const auto &layout : descriptor.setLayouts) {
        result &= (!layout.bindings.empty());

        for (const auto &binding : layout.bindings) {
            result &= (!binding.textureResourceDescriptors.empty());

            if (result) {
                if (!binding.textureResourceDescriptors.empty()) {
                    result &= (binding.resourceType == ResourceType::TEXTURE);

                    for (const auto &resourceDescriptor : binding.textureResourceDescriptors) {
                        result &= (resourceDescriptor.texture != nullptr);
                    }
                }
            }
        }
    }

    return result;
}

GLPipelineLayout::GLPipelineLayout(const PipelineLayoutDescriptor &descriptor) {
    for (const auto &setLayout : descriptor.setLayouts) {
        for (const auto &setLayoutBinding : setLayout.bindings) {
            switch (setLayoutBinding.resourceType) {
            case ResourceType::TEXTURE:
                m_textureBindings.emplace_back(setLayoutBinding);
                break;

            case ResourceType::INPUT_ATTACHMENT:
                m_inputAttachmentBindings.emplace_back(setLayoutBinding);
                break;
            }
        }
    }
}

GLTextureBinding::GLTextureBinding(const DescriptorSetLayoutBinding &binding) : GLResourceBinding{binding.bindingIndex},
                                                                                m_textures(binding.textureResourceDescriptors.size()) {
    for (size_t i = 0u; i < m_textures.size(); i++) {
        const auto &resourceDescriptor = binding.textureResourceDescriptors[i];
        m_textures[i] = std::static_pointer_cast<GLTexture>(resourceDescriptor.texture);
    }
}

void GLTextureBinding::bind() const {
    uint32_t currentTextureBindingIndex = m_bindingIndex;
    for (const auto &texture : m_textures) {
        texture->bind(currentTextureBindingIndex);
        currentTextureBindingIndex++;
    }
}

} // namespace nox
