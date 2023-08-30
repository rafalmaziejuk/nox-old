#include "unit_tests/mocks/mock_plugin.h"

void *MockPlugin::getProcedureAddress(std::string_view procedureName) const {
    lastRequestedProcedureName = procedureName.data();
    return procedureAddress;
}
