#include <gtest/gtest.h>

#include "silok/logger.hpp"

int main(int argc, char **argv)
{
    silok::logger::Logger::init("silok_test", "trace", "warn");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
