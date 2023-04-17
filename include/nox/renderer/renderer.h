#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>

#include <functional>
#include <memory>
#include <string_view>

namespace NOX {

class Renderer;
using RendererDeleter = std::function<void(Renderer *)>;

class NOX_EXPORT Renderer : public NonCopyable {
  public:
    [[nodiscard]] static std::unique_ptr<Renderer, RendererDeleter> create(std::string_view rendererName);
};

} // namespace NOX
