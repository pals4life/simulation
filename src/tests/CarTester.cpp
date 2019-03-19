//============================================================================
// @name        : CarTester.cpp
// @author      : Thomas Dooms
// @date        : 3/19/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <gtest/gtest.h>
#include "../datatypes/Car.h"

class CarTester : public ::testing::Test
{
protected:
    friend class Car;

    virtual void SetUp(){}
    virtual void TearDown(){}
};

TEST_F(CarTester, CarInit)
{
    EXPECT_DEATH(Car car(""       ,  0,  0), "License plate must be valid"    );
    EXPECT_DEATH(Car car("AAA-123", -1,  0), "Position must be greater than 0");
    EXPECT_DEATH(Car car("AAA-123",  0, -1), "Velocity must be greater than 0");
}

TEST_F(CarTester, CarMove)
{
    Car car("AAA-123", 0, 0);
    car.move(NULL, 100);

    ASSERT_EQ(car.getPosition(), 0);
    ASSERT_EQ(car.getVelocity(), 0);
    ASSERT_EQ(car.getAcceleration(), car.getMaxAcceleration());
}

TEST_F(CarTester, CarMove2)
{
    Car car("AAA-123", 0, 100);
    car.move(NULL, 100);

    ASSERT_EQ(car.getPosition(), 100);
    ASSERT_EQ(car.getVelocity(), 100);
    ASSERT_EQ(car.getAcceleration(), 0);
}