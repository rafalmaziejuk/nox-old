#pragma once

#include <nox/renderer/texture.h>

namespace NOX {

class GLTexture final : public Texture {
  public:
    GLTexture(const TextureDescriptor &descriptor);
    ~GLTexture() override;

    uint32_t getHandle() const { return m_handle; }
    const TextureDescriptor &getDescriptor() const { return m_descriptor; }

  private:
    void createTexture2D();

  private:
    TextureDescriptor m_descriptor;
    uint32_t m_handle{0u};
};

} // namespace NOX
