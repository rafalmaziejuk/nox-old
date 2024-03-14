#pragma once

#include <nox/export.h>
#include <nox/format.h>

#include <cstdint>

namespace nox {

struct BufferUsage {
    enum {
        STATIC = (1 << 0),
        DYNAMIC = (1 << 1)
    };
};

struct BufferDescriptor {
    uint32_t usage;
    uint32_t size;
    const void *data;
};

using VertexAttributes = std::vector<VertexAttributeFormat>;

struct VertexBufferDescriptor : BufferDescriptor {
    VertexAttributes vertexAttributes;
};

struct IndexBufferDescriptor : BufferDescriptor {
    VertexAttributeFormat format;
};

class NOX_EXPORT Buffer {
  public:
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;
    Buffer(Buffer &&) = delete;
    Buffer &operator=(Buffer &&) = delete;
    virtual ~Buffer() = default;

  protected:
    Buffer() = default;
};

} // namespace nox
