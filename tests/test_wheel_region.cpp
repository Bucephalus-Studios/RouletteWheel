#include "../classes/WheelRegion.hpp"
#include <gtest/gtest.h>
#include <string>

class WheelRegionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

TEST_F(WheelRegionTest, DefaultConstructor) {
    WheelRegion<int, double> region;
    EXPECT_EQ(region.getElement(), 0);
    EXPECT_EQ(region.getWeight(), 0.0);
}

TEST_F(WheelRegionTest, ParametricConstructorInt) {
    WheelRegion<int, int> region(42, 100);
    EXPECT_EQ(region.getElement(), 42);
    EXPECT_EQ(region.getWeight(), 100);
}

TEST_F(WheelRegionTest, ParametricConstructorString) {
    WheelRegion<std::string, double> region("apple", 0.75);
    EXPECT_EQ(region.getElement(), "apple");
    EXPECT_DOUBLE_EQ(region.getWeight(), 0.75);
}

TEST_F(WheelRegionTest, ParametricConstructorFloat) {
    WheelRegion<std::string, float> region("test", 3.14f);
    EXPECT_EQ(region.getElement(), "test");
    EXPECT_FLOAT_EQ(region.getWeight(), 3.14f);
}

TEST_F(WheelRegionTest, SetWeight) {
    WheelRegion<int, double> region(5, 1.0);
    EXPECT_DOUBLE_EQ(region.getWeight(), 1.0);

    region.setWeight(2.5);
    EXPECT_DOUBLE_EQ(region.getWeight(), 2.5);
    EXPECT_EQ(region.getElement(), 5);
}

TEST_F(WheelRegionTest, SetWeightNegative) {
    WheelRegion<int, int> region(10, 50);
    region.setWeight(-10);
    EXPECT_EQ(region.getWeight(), -10);
}

TEST_F(WheelRegionTest, CopyConstructor) {
    WheelRegion<std::string, int> original("data", 25);
    WheelRegion<std::string, int> copy = original;

    EXPECT_EQ(copy.getElement(), "data");
    EXPECT_EQ(copy.getWeight(), 25);
    EXPECT_EQ(original.getElement(), "data");
    EXPECT_EQ(original.getWeight(), 25);
}

TEST_F(WheelRegionTest, AssignmentOperator) {
    WheelRegion<int, double> region1(1, 1.5);
    WheelRegion<int, double> region2(2, 2.5);

    region2 = region1;

    EXPECT_EQ(region2.getElement(), 1);
    EXPECT_DOUBLE_EQ(region2.getWeight(), 1.5);
}

TEST_F(WheelRegionTest, ConstCorrectness) {
    const WheelRegion<std::string, float> region("const_test", 5.5f);

    // These should compile because getters are const
    EXPECT_EQ(region.getElement(), "const_test");
    EXPECT_FLOAT_EQ(region.getWeight(), 5.5f);
}

TEST_F(WheelRegionTest, ZeroWeight) {
    WheelRegion<char, int> region('A', 0);
    EXPECT_EQ(region.getElement(), 'A');
    EXPECT_EQ(region.getWeight(), 0);
}

TEST_F(WheelRegionTest, LargeWeight) {
    WheelRegion<int, long long> region(999, 1000000000LL);
    EXPECT_EQ(region.getElement(), 999);
    EXPECT_EQ(region.getWeight(), 1000000000LL);
}
