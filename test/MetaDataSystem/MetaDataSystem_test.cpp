#include <gtest/gtest.h>
#include <memory>
#include "Point.hpp"
#include "ImageContainer/MetaDataSystem.hpp"

using namespace FIPP;
// Demonstrate some basic assertions.
TEST(Creation, basics)
{
    // Expect equality.
    img::MetaDataValueNode<int> intNode(10);
    EXPECT_EQ(intNode.getType(), img::MetaTypes::INT);
    EXPECT_EQ(intNode.getValue(), 10);

    img::MetaDataValueNode<float> floatNode(50.00f);
    EXPECT_EQ(floatNode.getType(), img::MetaTypes::FLOAT);
    EXPECT_EQ(floatNode.getValue(), 50.00f);

    img::MetaDataValueNode<char> charNode('a');
    EXPECT_EQ(charNode.getType(), img::MetaTypes::CHAR);
    EXPECT_EQ(charNode.getValue(), 'a');

    img::MetaDataValueNode<bool> boolNode(false);
    EXPECT_EQ(boolNode.getType(), img::MetaTypes::BOOL);
    EXPECT_EQ(boolNode.getValue(), false);

    img::MetaDataValueNode<Point<int>> pointNode(Point<int>(10, -20));
    EXPECT_EQ(pointNode.getType(), img::MetaTypes::POINT_INT);
    EXPECT_EQ(pointNode.getValue().getXInt(), 10);
    EXPECT_EQ(pointNode.getValue().getYInt(), -20);

    img::MetaDataValueNode<std::string> stringNode(std::string("Hello World"));
    EXPECT_EQ(stringNode.getType(), img::MetaTypes::STRING);
    EXPECT_EQ(stringNode.getValue(), std::string("Hello World"));
}

TEST(Creation, vectors)
{
    // Expect equality.
    std::vector<int> intTest(10,-40);
    img::MetaDataVectorNode<int> intNode(intTest);
    EXPECT_EQ(intNode.getType(), img::MetaTypes::VECTOR_INT);
    const std::vector<int> vec = intNode.getValue(); 
    for(const auto val : vec){
        
        EXPECT_EQ(val, -40);
    }
    const std::vector<int>* vecPtr = intNode.getValuePtr();
    for(const auto valFromPtr : *vecPtr){
        
        EXPECT_EQ(valFromPtr, -40);
    }
    
}

TEST(Creation, map)
{
    // Expect equality.
    img::MetaDataMapNode mapNode;
    std::shared_ptr<img::MetaDataValueNode<int>> iNodePtr = std::make_shared<img::MetaDataValueNode<int>>(-10);
    std::vector<float> floatTest(10,-40.00f);
    std::shared_ptr<img::MetaDataVectorNode<float>> fNodeVecPtr = std::make_shared<img::MetaDataVectorNode<float>>(floatTest);
    mapNode.addNode("integer", iNodePtr);
    mapNode.addNode("vector", fNodeVecPtr);
    EXPECT_EQ(mapNode.getNode("integer")->getType(), img::MetaTypes::INT);
    std::shared_ptr<img::MetaDataValueNode<int>> iCompNodePtr = std::dynamic_pointer_cast<img::MetaDataValueNode<int>>(mapNode.getNode("integer"));
    EXPECT_EQ(iCompNodePtr->getValue(), -10);
    
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}