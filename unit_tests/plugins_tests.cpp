#include "plugins/plugin.h"

#include <gtest/gtest.h>

using namespace NOX;

TEST(PluginsTests, GivenPluginNameAndPluginFilenameCreationStrategyWhenCreatingPluginThenCorrectPluginFilenameIsSet) {
    constexpr auto pluginName = "pluginName";
    constexpr auto expectedPluginFilename = "pluginName.extension";
    auto mockPluginFilenameCreationStrategy = [](std::string_view name, [[maybe_unused]] std::string_view extension) -> std::string {
        constexpr auto pluginExtension = "extension";
        return std::string(name) + '.' + std::string(pluginExtension);
    };

    auto plugin = Plugin::create(pluginName, mockPluginFilenameCreationStrategy);
    ASSERT_NE(nullptr, plugin);

    EXPECT_EQ(expectedPluginFilename, plugin->getFilename());
}
