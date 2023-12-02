#pragma once

#include <nox/texture.h>

namespace NOX {

class GLTexture final : public Texture {
  public:
    [[nodiscard]] static bool validateInput(const TextureDescriptor &descriptor);

    explicit GLTexture(const TextureDescriptor &descriptor);
    ~GLTexture() override;

    uint32_t getHandle() const { return m_handle; }

    void bind(uint32_t index) const;

    void accept(TextureVisitor &visitor) const override;

  private:
    uint32_t m_handle{0u};
};

} // namespace NOX
