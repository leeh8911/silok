#include <gtest/gtest.h>

#include "silok/infra/logger.hpp"

int main(int argc, char **argv)
{
    silok::infra::SetUpLogger("silok_test", "trace", "warn");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
