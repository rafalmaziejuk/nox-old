#pragma once

#include <nox/export.h>
#include <nox/buffer_types.h>

namespace NOX {

struct BufferDescriptor {
    uint32_t usage;
    uint32_t size;
    const void *data;
};

class NOX_EXPORT Buffer {
  public:
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;
    virtual ~Buffer();

  protected:
    Buffer() = default;
};

} // namespace NOX
