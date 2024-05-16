#pragma once

#include "src/asserts.h"

namespace nox::test {

template <typename T>
class VariableBackup {
  public:
    VariableBackup() = default;
    VariableBackup(T *variable) : m_variable{variable} {
        m_variableOldValue = *variable;
    }

    ~VariableBackup() {
        *m_variable = m_variableOldValue;
    }

  private:
    T *m_variable{nullptr};
    T m_variableOldValue;
};

class DisableAssert {
  public:
    DisableAssert() {
        m_assertDisabled = true;
    }

  private:
    bool &m_assertDisabled{getAssertDisabled()};
    test::VariableBackup<bool> m_backup{&m_assertDisabled};
};

} // namespace nox::test
