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

TEST_F(CarTester, CarHappyDay)
{
    Car car("AAA-123", 0, 0);
    car.move(NULL, 100);
    ASSERT_EQ(car.getVelocity(), car.getMaxAcceleration());
}