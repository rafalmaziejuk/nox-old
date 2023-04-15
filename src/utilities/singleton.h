#pragma once

#include <nox/non_copyable.h>

namespace NOX {

template <typename T>
class Singleton : public NonCopyable {
  public:
    static T &instance() {
        static T instance{};
        return instance;
    }

  protected:
    Singleton() = default;
};

} // namespace NOX
