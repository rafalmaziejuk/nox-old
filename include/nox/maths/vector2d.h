#pragma once

#include <type_traits>

namespace NOX {

template <typename T>
struct Vector2D {
    static_assert(std::is_pod<T>::value);

    constexpr Vector2D() = default;

    constexpr Vector2D(T x_, T y_) : x{x_},
                                     y{y_} {}

    template <typename U>
    explicit constexpr Vector2D(const Vector2D<U> &other) : Vector2D{other.x, other.y} {
        static_assert(std::is_same<T, U>::value);
    }

    T x{};
    T y{};
};

} // namespace NOX
