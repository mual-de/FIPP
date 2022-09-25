#include <gtest/gtest.h>
#include "Point.hpp"

using namespace FIPP;
// Demonstrate some basic assertions.
TEST(AreaTest, UInt)
{
  // Expect equality.
  Point<unsigned int> pUInt(10, 20);
  EXPECT_EQ(pUInt.getArea(), 200);
}

TEST(AreaTest, Int)
{

  Point<int> pInt(10, -20);
  EXPECT_EQ(pInt.getArea(), -200);
}

TEST(AreaTest, Float)
{
  Point<float> pFloat(10.00f, 20.00f);
  EXPECT_EQ(pFloat.getArea(), 200.00f);
}

TEST(ValueTest, Int)
{
  // Expect equality.
  Point<int> pInt(10, -20);
  EXPECT_EQ(pInt.getXInt(), 10);
  EXPECT_EQ(pInt.getYInt(), -20);
  EXPECT_EQ(pInt.getXFloat(), 10.00f);
  EXPECT_EQ(pInt.getYFloat(), -20.00f);
}

TEST(ValueTest, UInt)
{
  // Expect equality.
  Point<unsigned int> pInt(10, 20);
  EXPECT_EQ(pInt.getXInt(), 10);
  EXPECT_EQ(pInt.getYInt(), 20);
  EXPECT_EQ(pInt.getXFloat(), 10.00f);
  EXPECT_EQ(pInt.getYFloat(), 20.00f);
}

TEST(ValueTest, Float)
{
  // Expect equality.
  Point<float> pInt(10.00f, 20.00f);
  EXPECT_EQ(pInt.getXInt(), 10);
  EXPECT_EQ(pInt.getYInt(), 20);
  EXPECT_EQ(pInt.getXFloat(), 10.00f);
  EXPECT_EQ(pInt.getYFloat(), 20.00f);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}