#include "src/plugins/plugin.h"
#include "src/plugins/plugin_interface.h"

#include "tests/utilities/config.h"

#include <gtest/gtest.h>

using namespace nox;

TEST(PluginsTests, WhenCallingPluginVersionMethodThenCorrectPluginVersionIsReturned) {
    const auto plugin = Plugin::create(test::dummyPluginName);
    ASSERT_NE(nullptr, plugin);

    EXPECT_EQ(NOX_PLUGIN_API_VERSION, plugin->pluginVersion());
}

TEST(PluginsTests, WhenCallingPluginRegisterMethodThenTrueIsReturned) {
    const auto plugin = Plugin::create(test::dummyPluginName);
    ASSERT_NE(nullptr, plugin);

    EXPECT_TRUE(plugin->pluginRegister());
}

TEST(PluginsTests, WhenCallingCreatePluginFilenameThenCorrectPluginFilenameIsReturned) {
    constexpr auto pluginName = "plugin";

    {
        constexpr auto expectedPluginFilename = "nox-plugin";
        constexpr auto usePrefix = false;
        constexpr auto usePostfix = false;

        const auto pluginFilename = createPluginName<usePrefix, usePostfix>(pluginName);
        EXPECT_EQ(expectedPluginFilename, pluginFilename);
    }

    {
        constexpr auto expectedPluginFilename = "libnox-plugin";
        constexpr auto usePrefix = true;
        constexpr auto usePostfix = false;

        const auto pluginFilename = createPluginName<usePrefix, usePostfix>(pluginName);
        EXPECT_EQ(expectedPluginFilename, pluginFilename);
    }

    {
        constexpr auto expectedPluginFilename = "nox-plugin-d";
        constexpr auto usePrefix = false;
        constexpr auto usePostfix = true;

        const auto pluginFilename = createPluginName<usePrefix, usePostfix>(pluginName);
        EXPECT_EQ(expectedPluginFilename, pluginFilename);
    }

    {
        constexpr auto expectedPluginFilename = "libnox-plugin-d";
        constexpr auto usePrefix = true;
        constexpr auto usePostfix = true;

        const auto pluginFilename = createPluginName<usePrefix, usePostfix>(pluginName);
        EXPECT_EQ(expectedPluginFilename, pluginFilename);
    }
}
