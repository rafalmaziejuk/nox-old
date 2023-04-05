#pragma once

#include <nox/non_copyable.h>

namespace NOX {

template <typename T>
class NOX_EXPORT Singleton : public NonCopyable {
  public:
    static T &instance() {
        static T instance{};
        return instance;
    }

  protected:
    Singleton() {}
};

} // namespace NOX
