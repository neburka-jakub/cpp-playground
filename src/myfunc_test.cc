#include <gtest/gtest.h>
#include "myfunc.h"

TEST(TestSuiteName, TestName) {
    EXPECT_EQ(my::sum(2, 3), 5);
}