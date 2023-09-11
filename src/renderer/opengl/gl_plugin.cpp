#include "plugins/plugin_interface.h"
#include "renderer/renderer_callback_registry.h"
#include "renderer/opengl/gl_renderer.h"
#include "renderer/opengl/nox_opengl_export.h"

#define NOX_PLUGIN_EXPORT NOX_OPENGL_EXPORT

NOX_PLUGIN_REGISTER(OpenGL) {
    auto createRenderer = []() -> NOX::Renderer * {
        return new NOX::GLRenderer{};
    };
    auto destroyRenderer = [](NOX::Renderer *renderer) {
        delete renderer;
        renderer = nullptr;
    };

    NOX::RendererCallbackRegistry::registerCallback(NOX::RendererBackend::OPENGL, {createRenderer, destroyRenderer});
    return true;
}
