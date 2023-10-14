#include <nox/logger.h>

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    NOX::Logger::initialize();
    NOX::Logger::setLevel(NOX::LogLevel::OFF);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
