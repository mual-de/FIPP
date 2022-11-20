#include <gtest/gtest.h>
#include "Point.hpp"
#include <yaml-cpp/yaml.h>

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

TEST(YamlTest, Get){
  YAML::Node testNode = Point<int>(10, -20).asYaml();
  EXPECT_EQ(testNode["x"].as<int>(), 10);
  EXPECT_EQ(testNode["y"].as<int>(), -20);

  YAML::Node testNodeFloat = Point<float>(10.00f, -20.00f).asYaml();
  EXPECT_EQ(testNodeFloat["x"].as<int>(), 10.00f);
  EXPECT_EQ(testNodeFloat["y"].as<int>(), -20.00f);
}

TEST(YamlTest, Set){
  YAML::Node testNode;
  testNode["x"] = 10;
  testNode["y"] = -20;
  Point<int> test = PointFactory::ptIntFromYAML(testNode);
  EXPECT_EQ(test.getX(), 10);
  EXPECT_EQ(test.getY(), -20);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}