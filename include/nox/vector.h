#pragma once

#include <array>
#include <cstddef>
#include <type_traits>

namespace nox {

template <typename Type, size_t componentsCount>
struct Vector {
    static_assert(std::is_pod<Type>::value);
    static_assert(componentsCount >= 2u && componentsCount <= 4u);

    constexpr Vector() = default;

    constexpr Vector(Type x, Type y) : values{{x, y}} {
        static_assert(componentsCount == 2u);
    }

    constexpr Vector(Type x, Type y, Type z) : values{{x, y, z}} {
        static_assert(componentsCount == 3u);
    }

    constexpr Vector(Type x, Type y, Type z, Type w) : values{{x, y, z, w}} {
        static_assert(componentsCount == 4u);
    }

    template <typename U>
    constexpr explicit Vector(const Vector<U, 2> &other) : values{{static_cast<Type>(other.x()),
                                                                   static_cast<Type>(other.y())}} {}

    template <typename U>
    constexpr explicit Vector(const Vector<U, 3> &other) : values{{static_cast<Type>(other.x()),
                                                                   static_cast<Type>(other.y()),
                                                                   static_cast<Type>(other.z())}} {}

    template <typename U>
    constexpr explicit Vector(const Vector<U, 4> &other) : values{{static_cast<Type>(other.x()),
                                                                   static_cast<Type>(other.y()),
                                                                   static_cast<Type>(other.z()),
                                                                   static_cast<Type>(other.w())}} {}

    bool operator==(const Vector<Type, componentsCount> &other) const { return (values == other.values); }
    bool operator!=(const Vector<Type, componentsCount> &other) const { return (values != other.values); }

    Type x() const { return values[0]; }
    Type y() const { return values[1]; }
    Type z() const {
        static_assert(componentsCount >= 3u && componentsCount <= 4u);
        return values[2];
    }
    Type w() const {
        static_assert(componentsCount == 4u);
        return values[3];
    }

    Type r() const { return x(); }
    Type g() const { return y(); }
    Type b() const { return z(); }
    Type a() const { return w(); }

    std::array<Type, componentsCount> values{};
};

template <typename Type>
using Vector2D = Vector<Type, 2>;

template <typename Type>
using Vector3D = Vector<Type, 3>;

template <typename Type>
using Vector4D = Vector<Type, 4>;

} // namespace nox
