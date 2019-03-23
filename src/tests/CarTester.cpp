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

TEST_F(CarTester, CarMove3)
{
    Car* car1 = new Car("AAA-123", 0, 100);
    Car* car2 = new Car("BBB-123", 0, 104);
    Car* car3 = new Car("BBB-123", 0, 200);

    EXPECT_DEATH(car1->move(car2, 100), "distance between vehicles must be greater than minVehicleDist");
    EXPECT_DEATH(car1->move(car3, -10), "Speedlimit must be greater than 0");

    delete car1;
    delete car2;
    delete car3;
}