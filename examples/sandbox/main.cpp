#include "sandbox_application.h"

int main() {
    auto application = std::make_unique<NOX::SandboxApplication>();
    application->initialize();
    application->run();

    return 0;
}
