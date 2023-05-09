#pragma once

#include <type_traits>

namespace NOX {

template <typename T>
struct Vector3D {
    static_assert(std::is_pod<T>::value);

    constexpr Vector3D() = default;

    constexpr Vector3D(T x, T y, T z) : x{x},
                                        y{y},
                                        z{z} {}

    template <typename U>
    explicit constexpr Vector3D(const Vector3D<U> &other) : Vector3D{other.x, other.y, other.z} {
        static_assert(std::is_same<T, U>::value);
    }

    T x{};
    T y{};
    T z{};
};

} // namespace NOX
