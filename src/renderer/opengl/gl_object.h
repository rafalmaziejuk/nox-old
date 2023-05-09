#pragma once

#include <cstdint>

namespace NOX {

class GLObject {
  public:
    uint32_t getHandle() const { return m_handle; }

  protected:
    uint32_t m_handle{0u};
};

} // namespace NOX
