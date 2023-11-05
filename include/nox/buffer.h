#pragma once

#include <nox/export.h>
#include <nox/resource.h>

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

class NOX_EXPORT Buffer : public Resource {
  public:
    virtual void bind() = 0;

  public:
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;
    Buffer(Buffer &&) = delete;
    Buffer &operator=(Buffer &&) = delete;
    ~Buffer() override = default;

  protected:
    Buffer() = default;
};

} // namespace NOX
