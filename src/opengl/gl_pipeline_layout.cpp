#include "opengl/gl_pipeline_layout.h"
#include "opengl/gl_texture.h"

namespace nox {

bool GLPipelineLayout::validateInput(const PipelineLayoutDescription &description) {
    bool result = true;

    for (const auto &layout : description.setLayouts) {
        result &= (!layout.bindings.empty());

        for (const auto &binding : layout.bindings) {
            result &= (!binding.textureResourceDescriptions.empty());

            if (result) {
                if (!binding.textureResourceDescriptions.empty()) {
                    result &= (binding.resourceType == ResourceType::TEXTURE);

                    for (const auto &resourceDescription : binding.textureResourceDescriptions) {
                        result &= (resourceDescription.texture != nullptr);
                    }
                }
            }
        }
    }

    return result;
}

GLPipelineLayout::GLPipelineLayout(PipelineLayoutDescription &description) {
    for (const auto &layout : description.setLayouts) {
        m_bindings.reserve(layout.bindings.size());
        m_bindings.insert(m_bindings.end(), layout.bindings.begin(), layout.bindings.end());
    }
}

void GLPipelineLayout::bind() const {
    size_t currentTextureResourceBindingIndex = 0u;

    for (const auto &binding : m_bindings) {
        if (binding.resourceType == ResourceType::TEXTURE) {
            for (const auto &textureResourceDescription : binding.textureResourceDescriptions) {
                const auto *glTexture = static_cast<const GLTexture *>(textureResourceDescription.texture.get());
                glTexture->bind(static_cast<uint32_t>(currentTextureResourceBindingIndex));
                currentTextureResourceBindingIndex++;
            }
        }
    }
}

} // namespace nox
