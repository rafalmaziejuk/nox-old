#include "src/opengl/gl_buffer.h"
#include "src/opengl/gl_vertex_array.h"

#include "tests/base/opengl/gl_test_environment.h"

#include <glad/gl.h>
#include <gtest/gtest.h>

using namespace nox;

struct GLBufferFixture : public tests::GLTestFixture {
    struct Vertex {
        float position[2];
        float textureCoordinates[2];
    };
    static constexpr Vertex vertices[]{
        {{0.5f, 0.5f}, {1.0f, 1.0f}},   // top right
        {{0.5f, -0.5f}, {1.0f, 0.0f}},  // bottom right
        {{-0.5f, -0.5f}, {0.0f, 0.0f}}, // bottom left
        {{-0.5f, 0.5f}, {0.0f, 1.0f}},  // top left
    };

    static constexpr uint32_t indices[]{0, 1, 3,
                                        1, 2, 3};
};

TEST_F(GLBufferFixture, GivenValidVertexBufferDescriptorWhenCallingCreateVertexBufferThenVertexBufferIsSuccessfullyCreated) {
    auto vertexArrayRegistry = GLVertexArrayRegistry::create();

    VertexBufferDescriptor vertexBufferDescriptor{};
    vertexBufferDescriptor.usage = BufferUsage::STATIC;
    vertexBufferDescriptor.size = sizeof(vertices);
    vertexBufferDescriptor.data = vertices;
    vertexBufferDescriptor.vertexAttributes = {
        VertexAttributeFormat::RG32F,
        VertexAttributeFormat::RG32F,
    };
    auto vertexBuffer = GLVertexBuffer::create(vertexBufferDescriptor, vertexArrayRegistry);
    ASSERT_NE(nullptr, vertexBuffer);
}
