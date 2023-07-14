#pragma once

namespace NOX {

template <typename T>
class Singleton {
  public:
    static T &instance() {
        static T instance{};
        return instance;
    }

  public:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

  protected:
    Singleton() = default;
};

} // namespace NOX
