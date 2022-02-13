#include "Vector.h"

#include <gtest/gtest.h>

TEST(VectorTest, constructor_default) {
  Vector vector = Vector();

  ASSERT_DOUBLE_EQ(0.0, vector.x);
  ASSERT_DOUBLE_EQ(0.0, vector.y);
  ASSERT_DOUBLE_EQ(0.0, vector.z);
}

TEST(VectorTest, constructor_values) {
  Vector vector = Vector(1.0, -1.0, 495.0024);

  ASSERT_DOUBLE_EQ(1.0, vector.x);
  ASSERT_DOUBLE_EQ(-1.0, vector.y);
  ASSERT_DOUBLE_EQ(495.0024, vector.z);
}

TEST(VectorTest, comparison_equal) {
  Vector lhs = Vector(1.0, -1.0, 495.0024);
  Vector rhs = Vector(1.0, -1.0, 495.0024);

  EXPECT_TRUE(lhs == lhs);
  EXPECT_TRUE(lhs == rhs);
  EXPECT_FALSE(lhs != rhs);
}

TEST(VectorTest, comparison_not_equal) {
  Vector lhs = Vector(1.0, -1.0, 495.0024);
  Vector rhs = Vector(1.00013, -1.0, 495.0024);

  EXPECT_FALSE(lhs == rhs);
  EXPECT_FALSE(lhs != lhs);
  EXPECT_TRUE(lhs != rhs);
}

TEST(VectorTest, operator_add) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);
  Vector rhs = Vector(0.20, 0.40, 75.0);

  Vector result = lhs + rhs;

  ASSERT_DOUBLE_EQ(1.45, result.x);
  ASSERT_DOUBLE_EQ(-1.1, result.y);
  ASSERT_DOUBLE_EQ(450.0005, result.z);
  EXPECT_NE(lhs, result);
}

TEST(VectorTest, operator_add_scalar) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);

  Vector result = lhs + 1000.0;

  ASSERT_DOUBLE_EQ(1001.25, result.x);
  ASSERT_DOUBLE_EQ(998.5, result.y);
  ASSERT_DOUBLE_EQ(1375.0005, result.z);
  EXPECT_NE(lhs, result);
}

TEST(VectorTest, operator_assignment_add) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);
  Vector rhs = Vector(0.20, 0.40, 75.0);

  Vector result = lhs += rhs;

  ASSERT_DOUBLE_EQ(1.45, result.x);
  ASSERT_DOUBLE_EQ(-1.1, result.y);
  ASSERT_DOUBLE_EQ(450.0005, result.z);
  EXPECT_EQ(lhs, result);
}

TEST(VectorTest, operator_assignment_add_scalar) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);

  Vector result = lhs += 1000.0;

  ASSERT_DOUBLE_EQ(1001.25, result.x);
  ASSERT_DOUBLE_EQ(998.5, result.y);
  ASSERT_DOUBLE_EQ(1375.0005, result.z);
  EXPECT_EQ(lhs, result);
}

TEST(VectorTest, operator_subtract) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);
  Vector rhs = Vector(0.20, 0.40, 75.0);

  Vector result = lhs - rhs;

  ASSERT_DOUBLE_EQ(1.05, result.x);
  ASSERT_DOUBLE_EQ(-1.9, result.y);
  ASSERT_DOUBLE_EQ(300.0005, result.z);
  EXPECT_NE(lhs, result);
}

TEST(VectorTest, operator_subtract_scalar) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);

  Vector result = lhs - 1000.0;

  ASSERT_DOUBLE_EQ(-998.75, result.x);
  ASSERT_DOUBLE_EQ(-1001.5, result.y);
  ASSERT_DOUBLE_EQ(-624.9995, result.z);
  EXPECT_NE(lhs, result);
}

TEST(VectorTest, operator_assignment_subtract) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);
  Vector rhs = Vector(0.20, 0.40, 75.0);

  Vector result = lhs -= rhs;

  ASSERT_DOUBLE_EQ(1.05, result.x);
  ASSERT_DOUBLE_EQ(-1.9, result.y);
  ASSERT_DOUBLE_EQ(300.0005, result.z);
  EXPECT_EQ(lhs, result);
}

TEST(VectorTest, operator_assignment_subtract_scalar) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);

  Vector result = lhs -= 1000.0;

  ASSERT_DOUBLE_EQ(-998.75, result.x);
  ASSERT_DOUBLE_EQ(-1001.5, result.y);
  ASSERT_DOUBLE_EQ(-624.9995, result.z);
  EXPECT_EQ(lhs, result);
}

TEST(VectorTest, operator_multiply) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);
  Vector rhs = Vector(2.0, 3.0, 5.0);

  Vector result = lhs * rhs;

  ASSERT_DOUBLE_EQ(2.5, result.x);
  ASSERT_DOUBLE_EQ(-4.5, result.y);
  ASSERT_DOUBLE_EQ(1875.0025, result.z);
  EXPECT_NE(lhs, result);
}

TEST(VectorTest, operator_multiply_scalar) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);

  Vector result = lhs * 0.1;

  ASSERT_DOUBLE_EQ(0.125, result.x);
  ASSERT_DOUBLE_EQ(-0.15, result.y);
  ASSERT_DOUBLE_EQ(37.50005, result.z);
  EXPECT_NE(lhs, result);
}

TEST(VectorTest, operator_assignment_multiply) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);
  Vector rhs = Vector(2.0, 3.0, 5.0);

  Vector result = lhs *= rhs;

  ASSERT_DOUBLE_EQ(2.5, result.x);
  ASSERT_DOUBLE_EQ(-4.5, result.y);
  ASSERT_DOUBLE_EQ(1875.0025, result.z);
  EXPECT_EQ(lhs, result);
}

TEST(VectorTest, operator_assignment_multiply_scalar) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);

  Vector result = lhs *= 0.1;

  ASSERT_DOUBLE_EQ(0.125, result.x);
  ASSERT_DOUBLE_EQ(-0.15, result.y);
  ASSERT_DOUBLE_EQ(37.50005, result.z);
  EXPECT_EQ(lhs, result);
}

TEST(VectorTest, operator_divide) {
  Vector lhs = Vector(122.44, -99.3, 375.0005);
  Vector rhs = Vector(2.0, 3.0, 5.0);

  Vector result = lhs / rhs;

  ASSERT_DOUBLE_EQ(61.22, result.x);
  ASSERT_DOUBLE_EQ(-33.1, result.y);
  ASSERT_DOUBLE_EQ(75.0001, result.z);
  EXPECT_NE(lhs, result);
}

TEST(VectorTest, operator_divide_scalar) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);

  Vector result = lhs / 10;

  ASSERT_DOUBLE_EQ(0.125, result.x);
  ASSERT_DOUBLE_EQ(-0.15, result.y);
  ASSERT_DOUBLE_EQ(37.50005, result.z);
  EXPECT_NE(lhs, result);
}

TEST(VectorTest, operator_assignment_divide) {
  Vector lhs = Vector(122.44, -99.3, 375.0005);
  Vector rhs = Vector(2.0, 3.0, 5.0);

  Vector result = lhs /= rhs;

  ASSERT_DOUBLE_EQ(61.22, result.x);
  ASSERT_DOUBLE_EQ(-33.1, result.y);
  ASSERT_DOUBLE_EQ(75.0001, result.z);
  EXPECT_EQ(lhs, result);
}

TEST(VectorTest, operator_assignment_divide_scalar) {
  Vector lhs = Vector(1.25, -1.50, 375.0005);

  Vector result = lhs /= 10;

  ASSERT_DOUBLE_EQ(0.125, result.x);
  ASSERT_DOUBLE_EQ(-0.15, result.y);
  ASSERT_DOUBLE_EQ(37.50005, result.z);
  EXPECT_EQ(lhs, result);
}

TEST(VectorTest, dot_product) {
  Vector lhs = Vector(1300.0, -3.2, 0.005);
  Vector rhs = Vector(2.0, -1.0, 10.0);

  double result = lhs.dot(rhs);
  ASSERT_DOUBLE_EQ(2603.25, result);
}

TEST(VectorTest, cross_product) {
  Vector lhs = Vector(-1.0, 1.0, 0.5);
  Vector rhs = Vector(1.0, 2.0, -1.0);

  Vector result = lhs.cross(rhs);

  ASSERT_DOUBLE_EQ(-2.0, result.x);
  ASSERT_DOUBLE_EQ(-0.5, result.y);
  ASSERT_DOUBLE_EQ(-3.0, result.z);
}

TEST(VectorTest, length) {
  Vector lhs = Vector(-23.32, 43.32, 28.98);

  double result = lhs.length();

  EXPECT_NEAR(57.098907169927521, result, 0.000000000001);
}

TEST(VectorTest, normalize_large_vector) {
  Vector vector = Vector(9984019.0, 9.32, -0.0023);

  double length = vector.normalize();

  EXPECT_NEAR(1.0, vector.x, 0.000001);
  EXPECT_NEAR(0.0, vector.y, 0.000001);
  EXPECT_NEAR(0.0, vector.z, 0.000001);
  EXPECT_NEAR(9984019.0, length, 0.00001);
}
