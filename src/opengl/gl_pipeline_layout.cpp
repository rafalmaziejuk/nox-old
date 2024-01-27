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
                m_bindings.emplace_back(std::make_unique<GLTextureResourceBinding>(setLayoutBinding));
                break;

            case ResourceType::INPUT_ATTACHMENT:
                m_bindings.emplace_back(std::make_unique<GLInputAttachmentResourceBinding>(setLayoutBinding));
                break;
            }
        }
    }
}

bool GLPipelineLayout::contains(uint32_t handle) const {
    for (const auto &binding : m_bindings) {
        const auto &handles = binding->getHandles();
        auto it = std::find_if(handles.begin(), handles.end(), [handle](uint32_t other) {
            return (other == handle);
        });

        if (it != handles.end()) {
            return true;
        }
    }

    return false;
}

GLTextureResourceBinding::GLTextureResourceBinding(const DescriptorSetLayoutBinding &binding) : GLResourceBinding{binding.bindingIndex},
                                                                                                m_textures(binding.textureResourceDescriptors.size()) {
    for (size_t i = 0u; i < m_textures.size(); i++) {
        const auto &resourceDescriptor = binding.textureResourceDescriptors[i];
        m_textures[i] = std::static_pointer_cast<GLTexture>(resourceDescriptor.texture);
    }
}

std::vector<uint32_t> GLTextureResourceBinding::getHandles() const {
    std::vector<uint32_t> handles(m_textures.size());
    for (const auto &texture : m_textures) {
        handles.push_back(texture->getHandle());
    }

    return handles;
}

void GLTextureResourceBinding::bind() const {
    uint32_t currentTextureBindingIndex = m_bindingIndex;
    for (const auto &texture : m_textures) {
        texture->bind(currentTextureBindingIndex);
        currentTextureBindingIndex++;
    }
}

} // namespace nox
