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

GLPipelineLayout::GLPipelineLayout(PipelineLayoutDescriptor &descriptor) {
    for (const auto &layout : descriptor.setLayouts) {
        m_bindings.reserve(layout.bindings.size());
        m_bindings.insert(m_bindings.end(), layout.bindings.begin(), layout.bindings.end());
    }
}

void GLPipelineLayout::bind() const {
    size_t currentTextureResourceBindingIndex = 0u;

    for (const auto &binding : m_bindings) {
        if (binding.resourceType == ResourceType::TEXTURE) {
            for (const auto &textureResourceDescriptor : binding.textureResourceDescriptors) {
                const auto *glTexture = static_cast<const GLTexture *>(textureResourceDescriptor.texture.get());
                glTexture->bind(static_cast<uint32_t>(currentTextureResourceBindingIndex));
                currentTextureResourceBindingIndex++;
            }
        }
    }
}

} // namespace nox
