#include "plugins/plugin_interface.h"
#include "renderer/renderer_factory_registry.h"
#include "renderer/opengl/gl_renderer.h"
#include "renderer/opengl/nox_opengl_export.h"

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
    registry[NOX::RendererBackend::OPENGL] = {createRenderer, destroyRenderer};

    return true;
}

NOX_PLUGIN_VERSION(OPENGL);
