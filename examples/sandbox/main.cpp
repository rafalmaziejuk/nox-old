#include "sandbox_application.h"

int main() {
    auto application = std::make_unique<nox::SandboxApplication>();
    application->initialize();
    application->run();

    return 0;
}
