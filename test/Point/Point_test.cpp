#include <gtest/gtest.h>
#include "Point.hpp"

using namespace FIPP;
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect equality.
  Point<unsigned int> pInt(10,20);
  EXPECT_EQ(pInt.getArea(), 200);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}