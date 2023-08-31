#include "plugins/plugin.h"

namespace NOX {

std::string Plugin::createFilename(std::string_view name, bool withPrefix, bool withPostfix) {
    NOX_ASSERT(name.empty());

    const auto *prefix = (withPrefix ? "lib" : "");
    const auto *postfix = (withPostfix ? "-d" : "");
    std::string result;
    result += prefix;
    result += name;
    result += postfix;
    return result;
}

std::string Plugin::createFilenameWithExtension(std::string_view name, std::string_view extension) {
    NOX_ASSERT(name.empty() || extension.empty());

    std::string result;
    result += name;
    result += '.';
    result += extension;
    return result;
}

} // namespace NOX
