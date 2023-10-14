#pragma once

#include <nox/texture.h>

namespace NOX {

class GLTexture final : public Texture {
  public:
    explicit GLTexture(const TextureDescriptor &descriptor);
    ~GLTexture() override;

    uint32_t getHandle() const;
    const TextureDescriptor &getDescriptor() const;

    void bind(uint32_t index) const;

  private:
    void createTexture2D() const;

  private:
    TextureDescriptor m_descriptor;
    uint32_t m_handle{0u};
};

} // namespace NOX
