#pragma once

#include "plugins/plugin.h"

using NOX::Plugin;

struct MockPlugin : public Plugin {
    void *getProcedureAddress(std::string_view procedureName) const override;

    void *procedureAddress{nullptr};
    mutable std::string lastRequestedProcedureName;
};
