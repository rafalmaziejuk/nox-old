#include "plugins/plugin.h"

#include "mocks/mock_plugin.h"

#include <gtest/gtest.h>

using namespace nox;

TEST(PluginsTests, WhenCallingCreatePluginFilenameThenCorrectPluginFilenameIsReturned) {
    constexpr auto pluginName = "plugin";
    constexpr auto pluginExtension = "extension";

    {
        constexpr auto expectedPluginFilename = "nox-plugin.extension";
        constexpr auto usePrefix = false;
        constexpr auto usePostfix = false;

        const auto pluginFilename = createPluginFilename<usePrefix, usePostfix>(pluginName, pluginExtension);
        EXPECT_EQ(expectedPluginFilename, pluginFilename);
    }

    {
        constexpr auto expectedPluginFilename = "libnox-plugin.extension";
        constexpr auto usePrefix = true;
        constexpr auto usePostfix = false;

        const auto pluginFilename = createPluginFilename<usePrefix, usePostfix>(pluginName, pluginExtension);
        EXPECT_EQ(expectedPluginFilename, pluginFilename);
    }

    {
        constexpr auto expectedPluginFilename = "nox-plugin-d.extension";
        constexpr auto usePrefix = false;
        constexpr auto usePostfix = true;

        const auto pluginFilename = createPluginFilename<usePrefix, usePostfix>(pluginName, pluginExtension);
        EXPECT_EQ(expectedPluginFilename, pluginFilename);
    }

    {
        constexpr auto expectedPluginFilename = "libnox-plugin-d.extension";
        constexpr auto usePrefix = true;
        constexpr auto usePostfix = true;

        const auto pluginFilename = createPluginFilename<usePrefix, usePostfix>(pluginName, pluginExtension);
        EXPECT_EQ(expectedPluginFilename, pluginFilename);
    }
}

TEST(PluginsTests, WhenCallingPluginRegisterAndPluginVersionMethodsThenCorrectValuesAreReturned) {
    const auto mockPlugin = Plugin::create("mockPlugin");
    ASSERT_NE(nullptr, mockPlugin);

    EXPECT_EQ(MockPlugin::expectedPluginRegisterOutput, mockPlugin->pluginRegister());
    EXPECT_EQ(MockPlugin::expectedPluginVersionOutput, mockPlugin->pluginVersion());
}
