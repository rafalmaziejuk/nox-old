#include "src/opengl/gl_shader.h"

#include "tests/base/opengl/gl_test_environment.h"

#include <glad/gl.h>
#include <gtest/gtest.h>

using namespace nox;

struct GLShaderFixture : public tests::GLTestFixture {
    static constexpr auto validShaderSource = R"(
            #version 460 core

            void main() {}
        )";
    static constexpr auto invalidShaderSource = R"(
            #version 460 core

            main() {}
        )";
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

    for (const auto &[shaderType, expectedShaderType] : testParams) {
        ShaderDescriptor shaderDescriptor{};
        shaderDescriptor.type = shaderType;

        const auto shader = GLShader::create(shaderDescriptor);
        ASSERT_NE(nullptr, shader);
        ASSERT_TRUE(shader->compile(validShaderSource));

        GLint glShaderType = GL_NONE;
        glGetShaderiv(shader->getHandle(), GL_SHADER_TYPE, &glShaderType);

        EXPECT_NE(0u, shader->getHandle());
        EXPECT_EQ(shaderType, shader->getType());
        EXPECT_EQ(expectedShaderType, glShaderType);
    }
}

TEST_F(GLShaderFixture, WhenDestroyingShaderThenShaderIsCorrectlyDestroyed) {
    ShaderDescriptor shaderDescriptor{};
    shaderDescriptor.type = ShaderType::VERTEX;

    uint32_t handle = 0u;
    {
        const auto shader = GLShader::create(shaderDescriptor);
        handle = shader->getHandle();
    }

    EXPECT_FALSE(glIsShader(handle));
}

TEST_F(GLShaderFixture, GivenInvalidShaderDescriptorWhenCallingCreateShaderThenNullptrIsReturned) {
    ShaderDescriptor shaderDescriptor{};
    shaderDescriptor.type = ShaderType::NONE;

    const auto shader = GLShader::create(shaderDescriptor);
    EXPECT_EQ(nullptr, shader);
}

TEST_F(GLShaderFixture, GivenValidShaderSourceWhenCallingCompileThenTrueIsReturned) {
    ShaderDescriptor shaderDescriptor{};
    shaderDescriptor.type = ShaderType::VERTEX;

    const auto shader = GLShader::create(shaderDescriptor);
    EXPECT_TRUE(shader->compile(validShaderSource));
}

TEST_F(GLShaderFixture, GivenInvalidShaderSourceWhenCallingCompileThenFalseIsReturnedAndShaderIsDeleted) {
    ShaderDescriptor shaderDescriptor{};
    shaderDescriptor.type = ShaderType::VERTEX;

    const auto shader = GLShader::create(shaderDescriptor);
    const auto handle = shader->getHandle();

    EXPECT_FALSE(shader->compile(invalidShaderSource));
    EXPECT_FALSE(glIsShader(handle));
}
