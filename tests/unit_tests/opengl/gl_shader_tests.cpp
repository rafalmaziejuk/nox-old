#include "src/opengl/gl_context.h"
#include "src/opengl/gl_shader.h"

#include "tests/base/window.h"

#include <glad/gl.h>
#include <gtest/gtest.h>

using namespace nox;

struct GLShaderFixture : public ::testing::Test {
    void SetUp() override {
        SurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.surfaceBackendDescriptor = window.surfaceBackendDescriptor;
        surfaceDescriptor.surfaceAttributesDescriptor = OpenGLSurfaceAttributesDescriptor{};

        context = GLContext::create(surfaceDescriptor);
        ASSERT_NE(nullptr, context);
    }

    tests::Window window{};
    std::unique_ptr<GLContext> context{nullptr};
};

TEST_F(GLShaderFixture, GivenValidShaderDescriptorAndSourceWhenCallingCreateShaderThenShaderIsSuccessfullyCreated) {
    using TestParams = std::array<std::pair<ShaderType, GLenum>, static_cast<size_t>(ShaderType::MAX) - 1>;
    constexpr TestParams testParams{{
        {ShaderType::VERTEX, GL_VERTEX_SHADER},
        {ShaderType::FRAGMENT, GL_FRAGMENT_SHADER},
        {ShaderType::TESS_CONTROL, GL_TESS_CONTROL_SHADER},
        {ShaderType::TESS_EVALUATION, GL_TESS_EVALUATION_SHADER},
        {ShaderType::GEOMETRY, GL_GEOMETRY_SHADER},
        {ShaderType::COMPUTE, GL_COMPUTE_SHADER},
    }};
    constexpr auto shaderSource = R"(
            #version 460 core

		    void main() {}
        )";

    for (const auto &[shaderType, expectedShaderType] : testParams) {
        ShaderDescriptor shaderDescriptor{};
        shaderDescriptor.type = shaderType;

        const auto shader = GLShader::create(shaderDescriptor, shaderSource);
        ASSERT_NE(nullptr, shader);

        GLint glShaderType = GL_NONE;
        glGetShaderiv(shader->getHandle(), GL_SHADER_TYPE, &glShaderType);

        EXPECT_NE(0u, shader->getHandle());
        EXPECT_EQ(shaderType, shader->getType());
        EXPECT_EQ(expectedShaderType, glShaderType);
    }
}

TEST_F(GLShaderFixture, GivenInvalidShaderDescriptorWhenCallingCreateShaderThenNullptrIsReturned) {
    constexpr auto shaderSource = R"(
            #version 460 core

            void main() {}
        )";

    ShaderDescriptor shaderDescriptor{};
    shaderDescriptor.type = ShaderType::NONE;

    const auto shader = GLShader::create(shaderDescriptor, shaderSource);
    EXPECT_EQ(nullptr, shader);
}

TEST_F(GLShaderFixture, GivenInvalidShaderSourceWhenCallingCreateShaderThenNullptrIsReturned) {
    constexpr auto shaderSource = R"(
            #version 460 core

            main() {}
        )";

    ShaderDescriptor shaderDescriptor{};
    shaderDescriptor.type = ShaderType::VERTEX;

    const auto shader = GLShader::create(shaderDescriptor, shaderSource);
    EXPECT_EQ(nullptr, shader);
}
