#include "asserts.h"
#include "renderer_factory_registry.h"
#include "plugins/plugin_interface.h"

#include <optional>

#if defined(NOX_BUILD_RENDERER_OPENGL)
NOX_PLUGIN_IMPORT(OPENGL);
#endif

namespace nox {

namespace {

std::unique_ptr<Plugin> createRendererPlugin(RendererBackend backend) {
#if defined(NOX_BUILD_RENDERER_OPENGL)
    if (backend == RendererBackend::OPENGL) {
        return NOX_PLUGIN_CREATE(OPENGL);
    }
#endif

    return nullptr;
}

} // namespace

struct RendererFactoryRegistry::Impl {
    std::optional<RendererFactory> rendererFactory{};
    std::unique_ptr<Plugin> rendererPlugin{nullptr};
    RendererBackend rendererBackend{RendererBackend::NONE};
};

RendererFactoryRegistry::RendererFactoryRegistry() : m_impl{new Impl} {}

RendererFactoryRegistry::~RendererFactoryRegistry() {
    delete m_impl;
    m_impl = nullptr;
}

RendererFactoryRegistry &RendererFactoryRegistry::instance() {
    static RendererFactoryRegistry instance{};
    return instance;
}

bool RendererFactoryRegistry::initialize(RendererBackend backend) {
    if (m_impl->rendererFactory.has_value()) {
        NOX_ENSURE_RETURN_FALSE_MSG(backend == m_impl->rendererBackend, "Creation of different API renderers isn't allowed");
        return true;
    }

    auto plugin = createRendererPlugin(backend);
    NOX_ENSURE_RETURN_FALSE_MSG(plugin != nullptr, "Couldn't create renderer plugin");
    NOX_ENSURE_RETURN_FALSE_MSG(plugin->pluginRegister(), "Couldn't register renderer plugin");
    NOX_ENSURE_RETURN_FALSE_MSG(plugin->pluginVersion() == NOX_PLUGIN_API_VERSION, "Plugin API version mismatch");
    NOX_ENSURE_RETURN_FALSE_MSG(m_impl->rendererFactory.has_value(), "Couldn't initialize renderer factory");
    m_impl->rendererPlugin = std::move(plugin);

    return true;
}

void RendererFactoryRegistry::registerFactory(RendererBackend backend, const RendererFactory &factory) {
    m_impl->rendererBackend = backend;
    m_impl->rendererFactory = factory;
}

const RendererFactory &RendererFactoryRegistry::getFactory(RendererBackend backend) const {
    NOX_ASSERT_MSG(m_impl->rendererFactory.has_value(), "Renderer factory hasn't been initialized");
    NOX_ASSERT_MSG(backend == m_impl->rendererBackend, "Renderer factory for given backend hasn't been initialized");

    return *(m_impl->rendererFactory);
}

} // namespace nox
