#include "src/opengl/gl_context.h"
#include "src/opengl/gl_program.h"
#include "src/opengl/gl_shader.h"

#include "tests/fixtures/window_fixture.h"

#include <glad/gl.h>
#include <gtest/gtest.h>

using namespace nox;

struct GLProgramFixture : public tests::WindowFixture,
                          public ::testing::Test {
    void SetUp() override {
        tests::WindowFixture::setUp();

        SurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.surfaceBackendDescriptor = surfaceBackendDescriptor;
        surfaceDescriptor.surfaceAttributesDescriptor = OpenGLSurfaceAttributesDescriptor{};

        context = GLContext::create(surfaceDescriptor);
        ASSERT_NE(nullptr, context);
    }

    void TearDown() override {
        tests::WindowFixture::tearDown();
    }

    std::unique_ptr<GLContext> context{nullptr};
};

TEST_F(GLProgramFixture, WhenCreatingProgramThenProgramIsSuccessfullyCreated) {
    GLProgram program{};

    EXPECT_NE(0u, program.getHandle());
}

TEST_F(GLProgramFixture, GivenCorrectShadersWhenCallingLinkProgramThenTrueIsReturned) {
    constexpr auto vertexShaderSource = R"(
            #version 460 core

            out gl_PerVertex {
	            vec4 gl_Position;
            };

		    void main() {
			    gl_Position = vec4(1.0); 
		    }
        )";
    constexpr auto fragmentShaderSource = R"(
            #version 460 core

            out vec4 fragmentColor;

            void main() {
                fragmentColor = vec4(1.0);
            }
        )";

    const auto vertexShader = GLShader::create({ShaderType::VERTEX}, vertexShaderSource);
    ASSERT_NE(nullptr, vertexShader);

    const auto fragmentShader = GLShader::create({ShaderType::FRAGMENT}, fragmentShaderSource);
    ASSERT_NE(nullptr, fragmentShader);

    GLProgram program{};
    program.attachShader(vertexShader->getHandle());
    program.attachShader(fragmentShader->getHandle());

    EXPECT_TRUE(program.link());
}

TEST_F(GLProgramFixture, GivenIncorrectShadersWhenCallingLinkProgramThenFalseIsReturned) {
    constexpr auto vertexShaderSource = R"(
            #version 460 core

            out gl_PerVertex {
	            vec4 gl_Position;
            };

		    void main() {
			    gl_Position = vec4(1.0); 
		    }
        )";

    const auto vertexShader1 = GLShader::create({ShaderType::VERTEX}, vertexShaderSource);
    ASSERT_NE(nullptr, vertexShader1);

    const auto vertexShader2 = GLShader::create({ShaderType::VERTEX}, vertexShaderSource);
    ASSERT_NE(nullptr, vertexShader2);

    GLProgram program{};
    program.attachShader(vertexShader1->getHandle());
    program.attachShader(vertexShader2->getHandle());

    EXPECT_FALSE(program.link());
}

TEST_F(GLProgramFixture, WhenCallingBindProgramThenCorrectProgramIsBound) {
    constexpr auto vertexShaderSource = R"(
            #version 460 core

            out gl_PerVertex {
	            vec4 gl_Position;
            };

		    void main() {
			    gl_Position = vec4(1.0); 
		    }
        )";
    constexpr auto fragmentShaderSource = R"(
            #version 460 core

            out vec4 fragmentColor;

            void main() {
                fragmentColor = vec4(1.0);
            }
        )";

    const auto vertexShader = GLShader::create({ShaderType::VERTEX}, vertexShaderSource);
    ASSERT_NE(nullptr, vertexShader);

    const auto fragmentShader = GLShader::create({ShaderType::FRAGMENT}, fragmentShaderSource);
    ASSERT_NE(nullptr, fragmentShader);

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
