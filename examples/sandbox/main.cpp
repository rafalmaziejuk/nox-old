#include <nox/renderer/renderer.h>

int main() {
    auto renderer = NOX::Renderer::create("OpenGL");
    NOX::Renderer::unload(renderer);

    return 0;
}
