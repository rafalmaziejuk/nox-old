#include "src/opengl/gl_program.h"
#include "src/opengl/gl_shader.h"

#include "tests/base/opengl/gl_test_environment.h"

#include <glad/gl.h>
#include <gtest/gtest.h>

using namespace nox;

struct GLProgramFixture : public tests::GLTestFixture {
    static constexpr auto vertexShaderSource = R"(
            #version 460 core

            out gl_PerVertex {
	            vec4 gl_Position;
            };

		    void main() {
			    gl_Position = vec4(1.0); 
		    }
        )";
    static constexpr auto fragmentShaderSource = R"(
            #version 460 core

            out vec4 fragmentColor;

            void main() {
                fragmentColor = vec4(1.0);
            }
        )";
};

TEST_F(GLProgramFixture, WhenCreatingProgramThenProgramIsSuccessfullyCreated) {
    GLProgram program{};

    EXPECT_NE(0u, program.getHandle());
}

TEST_F(GLProgramFixture, WhenDestroyingProgramThenProgramIsCorrectlyDestroyed) {
    uint32_t handle = 0u;
    {
        GLProgram program{};
        handle = program.getHandle();

        EXPECT_NE(0u, handle);
    }

    EXPECT_FALSE(glIsProgram(handle));
}

TEST_F(GLProgramFixture, GivenCorrectShadersWhenCallingAttachShaderThenShadersAreCorrectlyAttached) {
    const auto vertexShader = GLShader::create({ShaderType::VERTEX});
    ASSERT_NE(nullptr, vertexShader);
    ASSERT_TRUE(vertexShader->compile(vertexShaderSource));

    const auto fragmentShader = GLShader::create({ShaderType::FRAGMENT});
    ASSERT_NE(nullptr, fragmentShader);
    ASSERT_TRUE(fragmentShader->compile(fragmentShaderSource));

    GLProgram program{};
    const auto handle = program.getHandle();

    program.attachShader(vertexShader->getHandle());
    program.attachShader(fragmentShader->getHandle());

    constexpr auto expectedAttachedShadersCount = 2;
    GLint attachedShadersCount = 0;
    glGetProgramiv(handle, GL_ATTACHED_SHADERS, &attachedShadersCount);
    EXPECT_EQ(expectedAttachedShadersCount, attachedShadersCount);

    std::vector<GLuint> handles(static_cast<size_t>(attachedShadersCount));
    glGetAttachedShaders(handle, attachedShadersCount, nullptr, handles.data());
    EXPECT_EQ(vertexShader->getHandle(), handles[0]);
    EXPECT_EQ(fragmentShader->getHandle(), handles[1]);
}

TEST_F(GLProgramFixture, GivenCorrectShadersWhenCallingLinkThenTrueIsReturned) {
    const auto vertexShader = GLShader::create({ShaderType::VERTEX});
    ASSERT_NE(nullptr, vertexShader);
    ASSERT_TRUE(vertexShader->compile(vertexShaderSource));

    const auto fragmentShader = GLShader::create({ShaderType::FRAGMENT});
    ASSERT_NE(nullptr, fragmentShader);
    ASSERT_TRUE(fragmentShader->compile(fragmentShaderSource));

    GLProgram program{};
    program.attachShader(vertexShader->getHandle());
    program.attachShader(fragmentShader->getHandle());

    EXPECT_TRUE(program.link());
}

TEST_F(GLProgramFixture, GivenCorrectShadersWhenCallingLinkThenShadersAreCorrectlyDetached) {
    const auto vertexShader = GLShader::create({ShaderType::VERTEX});
    ASSERT_NE(nullptr, vertexShader);
    ASSERT_TRUE(vertexShader->compile(vertexShaderSource));

    const auto fragmentShader = GLShader::create({ShaderType::FRAGMENT});
    ASSERT_NE(nullptr, fragmentShader);
    ASSERT_TRUE(fragmentShader->compile(fragmentShaderSource));

    GLProgram program{};
    const auto handle = program.getHandle();

    program.attachShader(vertexShader->getHandle());
    program.attachShader(fragmentShader->getHandle());

    constexpr auto expectedAttachedShadersCount = 2;
    GLint attachedShadersCount = 0;
    glGetProgramiv(handle, GL_ATTACHED_SHADERS, &attachedShadersCount);
    EXPECT_EQ(expectedAttachedShadersCount, attachedShadersCount);

    EXPECT_TRUE(program.link());

    glGetProgramiv(handle, GL_ATTACHED_SHADERS, &attachedShadersCount);
    EXPECT_EQ(0, attachedShadersCount);
}

TEST_F(GLProgramFixture, GivenIncorrectShadersWhenCallingLinkThenFalseIsReturned) {
    const auto vertexShader1 = GLShader::create({ShaderType::VERTEX});
    ASSERT_NE(nullptr, vertexShader1);
    ASSERT_TRUE(vertexShader1->compile(vertexShaderSource));

    const auto vertexShader2 = GLShader::create({ShaderType::VERTEX});
    ASSERT_NE(nullptr, vertexShader2);
    ASSERT_TRUE(vertexShader2->compile(vertexShaderSource));

    GLProgram program{};
    program.attachShader(vertexShader1->getHandle());
    program.attachShader(vertexShader2->getHandle());

    EXPECT_FALSE(program.link());
}

TEST_F(GLProgramFixture, WhenCallingBindAndUnbindThenCorrectProgramIsBound) {
    const auto vertexShader = GLShader::create({ShaderType::VERTEX});
    ASSERT_NE(nullptr, vertexShader);
    ASSERT_TRUE(vertexShader->compile(vertexShaderSource));

    const auto fragmentShader = GLShader::create({ShaderType::FRAGMENT});
    ASSERT_NE(nullptr, fragmentShader);
    ASSERT_TRUE(fragmentShader->compile(fragmentShaderSource));

    GLProgram program{};
    program.attachShader(vertexShader->getHandle());
    program.attachShader(fragmentShader->getHandle());
    ASSERT_TRUE(program.link());

    const auto handle = program.getHandle();
    EXPECT_NE(0u, handle);

    program.bind();

    GLint currentlyBoundProgramHandle = GL_NONE;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentlyBoundProgramHandle);

    EXPECT_EQ(handle, currentlyBoundProgramHandle);

    program.unbind();
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentlyBoundProgramHandle);

    EXPECT_EQ(0u, currentlyBoundProgramHandle);
}
