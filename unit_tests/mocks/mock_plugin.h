#pragma once

#include "plugins/dynamic_plugin.h"
#include "plugins/plugin.h"
#include "plugins/plugin_interface.h"

class MockPlugin : public NOX::Plugin {
  public:
    MockPlugin(std::string_view name = "mockPlugin") : Plugin{name} {
        m_version = NOX_PLUGIN_API_VERSION;
    }
    ~MockPlugin() override = default;

    bool load() override {
        return loadSuccessful;
    }

    void setPluginVersion(uint8_t version) {
        m_version = version;
    }

    bool loadSuccessful{true};
};

class MockDynamicPlugin : public NOX::DynamicPlugin {
  public:
    MockDynamicPlugin() : DynamicPlugin{"mockDynamicPlugin"} {}
    ~MockDynamicPlugin() override = default;

    void *getProcedureAddress(std::string_view procedureName) const override {
        lastRequestedProcedureName = procedureName.data();
        return procedureAddress;
    }

    ConvertibleProcedureAddress getFunction(std::string_view name) const {
        return {getProcedureAddress(name)};
    }

    bool load() override {
        return true;
    }

    void *procedureAddress{nullptr};
    mutable std::string lastRequestedProcedureName;
};

class MockPluginManager : public NOX::PluginManager {};
