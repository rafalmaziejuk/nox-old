#pragma once

#include <nox/common.h>
#include <nox/export.h>

#include <cstdint>

namespace NOX {

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

struct VertexBufferDescriptor : BufferDescriptor {
    VertexFormat vertexFormat;
};

struct IndexBufferDescriptor : BufferDescriptor {
    Format format;
};

class NOX_EXPORT Buffer {
  public:
    virtual void bind() = 0;

  public:
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;
    Buffer(Buffer &&) = delete;
    Buffer &operator=(Buffer &&) = delete;
    virtual ~Buffer() = default;

  protected:
    Buffer() = default;
};

} // namespace NOX
