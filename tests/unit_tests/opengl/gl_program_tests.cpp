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

TEST_F(GLProgramFixture, GivenCorrectShadersWhenLinkingProgramThenTrueIsReturned) {
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

    GLProgram program{};
    const GLShader vertexShader{{ShaderType::VERTEX}};
    const GLShader fragmentShader{{ShaderType::FRAGMENT}};

    vertexShader.compile(vertexShaderSource);
    fragmentShader.compile(fragmentShaderSource);
    program.attachShader(vertexShader.getHandle());
    program.attachShader(fragmentShader.getHandle());

    EXPECT_TRUE(program.link());
}

TEST_F(GLProgramFixture, GivenIncorrectShadersWhenLinkingProgramThenFalseIsReturned) {
    constexpr auto vertexShaderSource = R"(
            #version 460 core

            out gl_PerVertex {
	            vec4 gl_Position;
            };

		    void main() {
			    gl_Position = vec4(1.0); 
		    }
        )";

    GLProgram program{};
    const GLShader vertexShader1{{ShaderType::VERTEX}};
    const GLShader vertexShader2{{ShaderType::VERTEX}};

    vertexShader1.compile(vertexShaderSource);
    vertexShader2.compile(vertexShaderSource);
    program.attachShader(vertexShader1.getHandle());
    program.attachShader(vertexShader2.getHandle());

    EXPECT_FALSE(program.link());
}
