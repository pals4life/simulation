//============================================================================
// @name        : RoadTester.cpp
// @author      : Mano Marichal
// @date        : 3/25/19
// @version     :
// @copyright   : BA1 Informatica - Mano Marichal - University of Antwerp
// @description :
//============================================================================

#include <gtest/gtest.h>
#include "../datatypes/Road.h"
#include "../datatypes/Car.h"

class RoadTester : public ::testing::Test
{
protected:
    friend class Road;

    virtual void SetUp() {}
    virtual void TearDown() {}

};

TEST_F(RoadTester, RoadInit)
{
    EXPECT_DEATH(Road road("", NULL, -100, 100), "Assertion `Failed to construct road: length must be greater than 0' failed.");
    EXPECT_DEATH(Road road("E13", NULL, -100, 100), "Assertion `Failed to construct road: length must be greater than 0' failed.");
    EXPECT_DEATH(Road road("E13", NULL, 100, -100), "Assertion `Failed to construct road: speed limit must be greater than 0' failed.");

    Road testRoad("E13", NULL, 100, 100);
    ASSERT_TRUE(testRoad.properlyInitialized());

}

TEST_F(RoadTester, RoadUpdate1)
{
    Road* testRoad = new Road("E13", NULL, 100, 100);
    Car* testCar = new Car("12R3", 30, 30);

    testRoad->enqueue(testCar);
    testRoad->update();
    ASSERT_FALSE(testRoad->isEmpty());

    delete testRoad;

}

TEST_F(RoadTester, RoadUpdate2)
{
    Road* testRoad = new Road("E13", NULL, 100, 100);
    Car* testCar = new Car("12R3", 50, 60);

    testRoad->enqueue(testCar);
    testRoad->update();
    ASSERT_TRUE(testRoad->isEmpty());

    delete testRoad;

}

TEST_F(RoadTester, RoadUpdate3)
{
    Road* testRoad = new Road("E13", NULL, 100, 50);
    Car* testCar = new Car("12R3", 30, 40);

    testRoad->enqueue(testCar);
    testRoad->update();
    testRoad->update();
    ASSERT_TRUE(testRoad->isEmpty());

    delete testRoad;

}


TEST_F(RoadTester, RoadUpdate4)
{
    Road* testRoad = new Road("E13", NULL, 100, 50);
    Car* testCar0 = new Car("12R3", 50, 30);
    Car* testCar1 = new Car("AE-12", 40, 30);

    testRoad->enqueue(testCar0);
    testRoad->enqueue(testCar1);

    testRoad->update();
    ASSERT_EQ(testCar0->getPosition(), 80);
    ASSERT_EQ(testCar1->getPosition(), 70);

    delete testRoad;

}


TEST_F(RoadTester, RoadUpdate5)
{
    testing::internal::CaptureStderr();

    Road* testRoad = new Road("E13", NULL, 100, 50);
    Car* testCar0 = new Car("12R3", 50, 10);
    Car* testCar1 = new Car("AE-12", 40, 30);

    testRoad->enqueue(testCar0);
    testRoad->enqueue(testCar1);

    testRoad->update();
    ASSERT_EQ(testCar0->getPosition(), 60);
    ASSERT_EQ(testCar1->getPosition(), 70);

    std::cerr << testing::internal::GetCapturedStderr() << " in test 5\n";

    delete testRoad;
}
TEST_F(RoadTester, RoadUpdate6)
{
    Road* testRoad = new Road("E13", NULL, 1000, 100);
    Car* testCar = new Car("12R3", 0, 120);

    testRoad->enqueue(testCar);
    testRoad->update();
}
