#include <nox/renderer/renderer.h>
#include <nox/window/event.h>
#include <nox/window/window.h>

using namespace NOX;

class SandboxApplication {
  public:
    SandboxApplication() {
        WindowDescriptor windowDescriptor{};
        windowDescriptor.title = "Sandbox Example";
        windowDescriptor.size = {800u, 600u};
        windowDescriptor.isVisible = true;
        windowDescriptor.isCentered = true;
        windowDescriptor.isResizable = true;
        m_window = Window::create(windowDescriptor);

        EventDispatcher eventDispatcher;
        eventDispatcher.closeEventCallback = [this]() {
            m_isRunning = false;
            return true;
        };
        m_window->pushBackEventDispatcher(eventDispatcher);

        RendererDescriptor rendererDescriptor;
        rendererDescriptor.api = RendererAPI::OPENGL;
        m_renderer = Renderer::create(rendererDescriptor);
    }

    void run() {
        while (m_isRunning) {
            m_window->processEvents();
        }
    }

  private:
    bool m_isRunning{true};
    std::unique_ptr<Window> m_window{nullptr};
    std::unique_ptr<Renderer, RendererDeleter> m_renderer{nullptr};
};

int main() {
    auto application = std::make_unique<SandboxApplication>();
    application->run();

    return 0;
}
