#pragma once

#include "src/asserts.h"

namespace nox::tests {

struct AssertDisabledFixture {
    AssertDisabledFixture() {
        auto &assertDisabled = getAssertDisabled();
        assertDisabled = true;
    }

    virtual ~AssertDisabledFixture() {
        auto &assertDisabled = getAssertDisabled();
        assertDisabled = false;
    }
};

} // namespace nox::tests
