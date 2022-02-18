#include <gtest/gtest.h>

TEST(EmptyTest, passing_test) {
  ASSERT_DOUBLE_EQ(0.0, 0.0);
}

TEST(EmptyTest, failing_test) {
  ASSERT_DOUBLE_EQ(0.0, 1.0);
}
