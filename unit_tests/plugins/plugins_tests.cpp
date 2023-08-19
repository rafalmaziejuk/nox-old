#include "unit_tests/mocks/mock_plugin.h"

#include "plugins/plugin.h"

#include <gtest/gtest.h>

using namespace NOX;

TEST(Plugins, GivenPluginFilenameWhenCreatingPluginFilenameThenCorrectPluginFilenameIsReturned) {
    constexpr auto pluginName = "pluginName";

    {
        constexpr auto expectedPluginName = "pluginName";

        auto filename = Plugin::createFilename(pluginName, false, false);
        EXPECT_STREQ(expectedPluginName, filename.c_str());
    }

    {
        constexpr auto expectedPluginName = "libpluginName";

        auto filename = Plugin::createFilename(pluginName, true, false);
        EXPECT_STREQ(expectedPluginName, filename.c_str());
    }

    {
        constexpr auto expectedPluginName = "pluginName-d";

        auto filename = Plugin::createFilename(pluginName, false, true);
        EXPECT_STREQ(expectedPluginName, filename.c_str());
    }

    {
        constexpr auto expectedPluginName = "libpluginName-d";

        auto filename = Plugin::createFilename(pluginName, true, true);
        EXPECT_STREQ(expectedPluginName, filename.c_str());
    }
}

TEST(Plugins, GivenPluginFilenameAndExtensionWhenCreatingPluginFilenameWithExtensionThenCorrectPluginFilenameIsReturned) {
    constexpr auto name = "pluginName";
    constexpr auto extension = "extension";
    constexpr auto expectedPluginName = "pluginName.extension";

    auto filename = Plugin::createFilenameWithExtension(name, extension);
    EXPECT_STREQ(expectedPluginName, filename.c_str());
}

TEST(Plugins, WhenCallingIndexOperatorThenConvertibleProcedureAddressObjectToFunctionIsReturned) {
    NOX_DECLARE_PLUGIN_FUNCTION(PluginFunctionA, void, uint32_t);
    NOX_DECLARE_PLUGIN_FUNCTION(PluginFunctionB, uint32_t, void);

    MockPlugin mockPlugin;

    uint32_t variableA;
    mockPlugin.procedureAddress = &variableA;
    PluginFunctionA functionA = mockPlugin["functionA"];
    EXPECT_STREQ("functionA", mockPlugin.lastRequestedProcedureName.c_str());
    EXPECT_EQ(&variableA, reinterpret_cast<void *>(functionA));

    uint32_t variableB;
    mockPlugin.procedureAddress = &variableB;
    PluginFunctionB functionB = mockPlugin["functionB"];
    EXPECT_STREQ("functionB", mockPlugin.lastRequestedProcedureName.c_str());
    EXPECT_EQ(&variableB, reinterpret_cast<void *>(functionB));
}
