#include <nox/vector.h>

#include <string_view>

namespace nox::base {

struct Image {
    explicit Image(std::string_view filename);
    ~Image();

    void *data{nullptr};
    Vector2D<uint32_t> size{};
    uint32_t componentsCount{};
};

} // namespace nox::base
