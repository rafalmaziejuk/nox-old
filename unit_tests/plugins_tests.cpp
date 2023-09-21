#include "unit_tests/mocks/mock_plugin.h"

#include <gtest/gtest.h>

using namespace NOX;

TEST(PluginsTests, GivenPluginNameAndPluginFilenameCreationStrategyWhenCreatingDynamicPluginThenCorrectPluginFilenameIsSet) {
    constexpr auto pluginName = "pluginName";
    constexpr auto expectedPluginFilename = "pluginName.extension";
    auto mockPluginFilenameCreationStrategy = [](std::string_view name, [[maybe_unused]] std::string_view extension) -> std::string {
        constexpr auto pluginExtension = "extension";
        return std::string(name) + '.' + std::string(pluginExtension);
    };

    auto plugin = DynamicPlugin::create(pluginName, mockPluginFilenameCreationStrategy);
    ASSERT_NE(nullptr, plugin);

    EXPECT_EQ(expectedPluginFilename, plugin->getFilename());
}

TEST(PluginsTests, WhenCallingDynamicPluginGetFunctionThenConvertibleProcedureAddressObjectToFunctionIsReturned) {
    using PluginFunctionA = void (*)(uint32_t);
    using PluginFunctionB = uint32_t (*)();

    MockDynamicPlugin mockPlugin;

    uint32_t variableA;
    mockPlugin.procedureAddress = &variableA;
    PluginFunctionA functionA = mockPlugin.getFunction("functionA");
    EXPECT_STREQ("functionA", mockPlugin.lastRequestedProcedureName.c_str());
    EXPECT_EQ(&variableA, reinterpret_cast<void *>(functionA));

    uint32_t variableB;
    mockPlugin.procedureAddress = &variableB;
    PluginFunctionB functionB = mockPlugin.getFunction("functionB");
    EXPECT_STREQ("functionB", mockPlugin.lastRequestedProcedureName.c_str());
    EXPECT_EQ(&variableB, reinterpret_cast<void *>(functionB));
}

class PluginManagerTestsFixture : public ::testing::Test {
  public:
    MockPluginManager pluginManager;
};

TEST_F(PluginManagerTestsFixture, GivenNotLoadedAndValidPluginWhenCallingRegisterPluginThenTrueIsReturned) {
    auto mockPlugin = std::make_unique<MockPlugin>();
    mockPlugin->loadSuccessful = true;

    EXPECT_TRUE(pluginManager.registerPlugin(std::move(mockPlugin)));
}

TEST_F(PluginManagerTestsFixture, GivenNotLoadedAndInvalidPluginWhenCallingRegisterPluginThenFalseIsReturned) {
    auto mockPlugin = std::make_unique<MockPlugin>();
    mockPlugin->loadSuccessful = false;

    EXPECT_FALSE(pluginManager.registerPlugin(std::move(mockPlugin)));
}

TEST_F(PluginManagerTestsFixture, GivenNotLoadedAndValidPluginWithCompatibleApiVersionWhenCallingRegisterPluginThenTrueIsReturned) {
    auto mockPlugin = std::make_unique<MockPlugin>();
    mockPlugin->loadSuccessful = true;
    mockPlugin->setPluginVersion(NOX_PLUGIN_API_VERSION);

    EXPECT_TRUE(pluginManager.registerPlugin(std::move(mockPlugin)));
}

TEST_F(PluginManagerTestsFixture, GivenNotLoadedAndValidPluginWithIncompatibleApiVersionWhenCallingRegisterPluginThenFalseIsReturned) {
    auto mockPlugin = std::make_unique<MockPlugin>();
    mockPlugin->loadSuccessful = true;
    mockPlugin->setPluginVersion(0u);

    EXPECT_FALSE(pluginManager.registerPlugin(std::move(mockPlugin)));
}

TEST_F(PluginManagerTestsFixture, GivenTwoValidPluginsWithTheSameNameWhenCallingRegisterPluginThenTrueIsReturned) {
    constexpr auto mockPluginName = "mockPlugin";

    auto mockPlugin1 = std::make_unique<MockPlugin>(mockPluginName);
    mockPlugin1->loadSuccessful = true;
    EXPECT_TRUE(pluginManager.registerPlugin(std::move(mockPlugin1)));

    auto mockPlugin2 = std::make_unique<MockPlugin>(mockPluginName);
    mockPlugin2->loadSuccessful = true;
    EXPECT_TRUE(pluginManager.registerPlugin(std::move(mockPlugin2)));
}
