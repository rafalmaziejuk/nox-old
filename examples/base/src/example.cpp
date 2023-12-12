#include <base/example.h>

namespace nox::base {

void Example::run() {
    while (!m_window.shouldClose()) {
        m_window.processEvents();

        onUpdate();

        m_swapchain->present();
    }
}

} // namespace nox::base
