#pragma once

#include <nox/texture.h>

namespace nox {

class GLTexture : public Texture {
  public:
    explicit GLTexture(TextureType type);
    ~GLTexture() override;

    uint32_t getHandle() const { return m_handle; }

    void bind(uint32_t index) const;

    void accept(TextureVisitor &visitor) const override;

  protected:
    uint32_t m_handle{0u};
};

class GLTexture2D final : public GLTexture {
  public:
    [[nodiscard]] static bool validateInput(const Texture2DDescriptor &descriptor);

    explicit GLTexture2D(const Texture2DDescriptor &descriptor);

    TextureType getTextureType() const override;
};

} // namespace nox
