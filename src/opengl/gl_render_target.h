#pragma once

#include <nox/render_target.h>

namespace nox {

class GLTexture;

class GLRenderTargetBase : public RenderTarget {
  public:
    void bind() const;

    virtual void clear(const Vector4D<int32_t> &color, uint8_t index) const;
    virtual void clear(const Vector4D<uint32_t> &color, uint8_t index) const;
    virtual void clear(const Vector4D<float> &color, uint8_t index) const;
    void clear(float depth) const;
    void clear(uint32_t stencil) const;
    void clear(float depth, uint32_t stencil) const;

  protected:
    uint32_t m_handle{0u};
};

class GLDefaultRenderTarget final : public GLRenderTargetBase {
  public:
    void clear(const Vector4D<int32_t> &color, uint8_t index) const override;
    void clear(const Vector4D<uint32_t> &color, uint8_t index) const override;
    void clear(const Vector4D<float> &color, uint8_t index) const override;
};

class GLRenderTarget final : public GLRenderTargetBase {
  public:
    explicit GLRenderTarget(const RenderTargetDescriptor &descriptor);
    ~GLRenderTarget() override;

  private:
    void createColorAttachment(const Texture &texture, uint32_t attachmentPoint);
    void createDepthStencilAttachment(ImageFormat format);

  private:
    std::vector<std::unique_ptr<GLTexture>> m_depthStencilAttachments{};
    std::vector<uint32_t> m_attachmentPoints{};
    Vector2D<uint32_t> m_size{};
    uint8_t m_colorAttachmentsCount{0u};
    uint8_t m_depthStencilAttachmentsCount{0u};
};

} // namespace nox
