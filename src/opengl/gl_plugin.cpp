#include "renderer_factory_registry.h"
#include "plugins/plugin_interface.h"
#include "opengl/gl_renderer.h"
#include "opengl/nox_opengl_export.h"

#define NOX_PLUGIN_EXPORT NOX_OPENGL_EXPORT

NOX_PLUGIN_REGISTER(OPENGL) {
    auto createRenderer = []() -> NOX::Renderer * {
        return new NOX::GLRenderer{};
    };
    auto destroyRenderer = [](NOX::Renderer *renderer) -> void {
        delete renderer;
        renderer = nullptr;
    };

    auto &registry = NOX::RendererFactoryRegistry::instance();
    registry.registerFactory(NOX::RendererBackend::OPENGL, {createRenderer, destroyRenderer});

    return true;
}

NOX_PLUGIN_VERSION(OPENGL);
