#pragma once

#include <nox/buffer_types.h>
#include <nox/export.h>
#include <nox/resource.h>

namespace NOX {

struct BufferDescriptor {
    uint32_t usage;
    uint32_t size;
    const void *data;
};

class NOX_EXPORT Buffer : public Resource {
  public:
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;
    ~Buffer() override;

  protected:
    Buffer() = default;
};

} // namespace NOX
