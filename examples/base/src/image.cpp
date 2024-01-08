#include "base/image.h"

#include <stb_image/stb_image.h>

namespace nox::base {

Image::Image(std::string_view filename) {
    int32_t width{}, height{}, channels{};

    data = stbi_load(filename.data(),
                     &width,
                     &height,
                     &channels,
                     0);

    size = {static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};
    componentsCount = static_cast<uint32_t>(channels);
}

Image::~Image() {
    stbi_image_free(data);
}

} // namespace nox::base
