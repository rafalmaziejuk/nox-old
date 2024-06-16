#pragma once

#include "src/opengl/gl_context.h"

#include "tests/base/window.h"

#include <gtest/gtest.h>

namespace nox::tests {

struct GLTestEnvironment {
  public:
    [[nodiscard]] static const GLTestEnvironment &instance();

  public:
    GLTestEnvironment(const GLTestEnvironment &) = delete;
    GLTestEnvironment &operator=(const GLTestEnvironment &) = delete;
    GLTestEnvironment(GLTestEnvironment &&) = delete;
    GLTestEnvironment &operator=(GLTestEnvironment &&) = delete;

  private:
    GLTestEnvironment();

  public:
    std::unique_ptr<tests::Window> window{nullptr};
    std::unique_ptr<GLContext> context{nullptr};
};

struct GLTestFixture : public ::testing::Test {
    void SetUp() override {
        environment.context->makeCurrent();
    }
    
    const tests::GLTestEnvironment &environment{GLTestEnvironment::instance()};
};

} // namespace nox::tests
