#pragma once

#include <nox/export.h>
#include <nox/renderer/buffer_types.h>

namespace NOX {

struct BufferDescriptor {
    uint32_t size;
    const void *data;
    uint8_t accessMethod;
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
