#include "sandbox_example.h"

int main() {
    auto example = std::make_unique<nox::SandboxExample>();
    example->initialize();
    example->run();

    return 0;
}
