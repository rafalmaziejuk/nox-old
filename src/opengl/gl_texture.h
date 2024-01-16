#pragma once

#include <nox/texture.h>

namespace nox {

class GLTexture : public Texture {
  public:
    GLTexture(const TextureDescription &description, TextureType type);
    ~GLTexture() override;

    TextureType getType() const override;
    ImageFormat getFormat() const override;

    [[nodiscard]] uint32_t getHandle() const { return m_handle; }

    void bind(uint32_t index) const;

  protected:
    TextureType m_type;
    ImageFormat m_format;
    uint32_t m_handle{0u};
};

class GLTexture2D final : public GLTexture {
  public:
    [[nodiscard]] static bool validateInput(const Texture2DDescription &description);

    GLTexture2D(const Texture2DDescription &description);

    void write(const TextureWriteDescription &description) override;

    [[nodiscard]] bool isPresentable() const { return m_isPresentable; }
    void setPresentable() { m_isPresentable = true; }

  private:
    bool m_isPresentable{false};
};

} // namespace nox
