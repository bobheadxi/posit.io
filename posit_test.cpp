#include "posit.h"

#include <gtest/gtest.h>

TEST(TestPositInit, TestSuccess) {
    EXPECT_EQ(posit::init(), 1);
}
