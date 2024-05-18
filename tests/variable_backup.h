#pragma once

namespace nox::tests {

template <typename T>
class VariableBackup {
  public:
    VariableBackup(T *variable) 
    : m_variable{variable},
                                  m_variableBackupValue{*variable} {}

    ~VariableBackup() {
        *m_variable = m_variableBackupValue;
    }

  private:
    T *m_variable{nullptr};
    T m_variableBackupValue;
};

} // namespace nox::tests
