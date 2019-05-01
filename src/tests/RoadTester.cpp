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
#include "../datatypes/vehicles/Car.h"

class RoadTester : public ::testing::Test
{
protected:
    friend class Road;

    virtual void SetUp() {}
    virtual void TearDown() {}

};

TEST_F(RoadTester, RoadInit)
{
    const Zone* zone = new Zone(0, 100);
    std::vector<const Zone*> zones = std::vector<const Zone*>(1, zone);
    std::vector<const Zone*> empty = std::vector<const Zone*>();
    std::vector<const BusStop*> stops = std::vector<const BusStop*>();
    std::vector<const TrafficLight*> lights = std::vector<const TrafficLight*>();

    EXPECT_DEATH(Road road(""   , NULL, 100, 1, zones, stops, lights), "Assertion `Failed to construct road: name can not be empty' failed.");
    EXPECT_DEATH(Road road("E13", NULL,-100, 1, zones, stops, lights), "Assertion `Failed to construct road: length must be greater than 0' failed.");
    EXPECT_DEATH(Road road("E13", NULL, 100,-1, zones, stops, lights), "Assertion `Failed to construct road: must at least have 1 lane or less than 100' failed.");
    EXPECT_DEATH(Road road("E13", NULL, 100, 1, empty, stops, lights), "Assertion `Failed to construct road: must have at least 1 speed zone' failed");

    Road testRoad("E13", NULL, 100, 1, zones, stops, lights);
    ASSERT_TRUE(testRoad.properlyInitialized());
}

TEST_F(RoadTester, RoadUpdate1)
{
    const Zone* zone = new Zone(0, 100);
    Road* testRoad = new Road("E13", NULL, 100, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    IVehicle* testCar = new Car("12R3", 30, 30);

    testRoad->enqueue(testCar);
    testRoad->updateVehicles();
    ASSERT_FALSE(testRoad->isEmpty());
}

TEST_F(RoadTester, RoadUpdate2)
{
    const Zone* zone = new Zone(0, 100);
    Road* testRoad = new Road("E13", NULL, 100, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    IVehicle* testCar = new Car("12R3", 50, 60);

    testRoad->enqueue(testCar);
    testRoad->updateVehicles();
    ASSERT_TRUE(testRoad->isEmpty());
}

TEST_F(RoadTester, RoadUpdate3)
{
    const Zone* zone = new Zone(0, 100);
    Road* testRoad = new Road("E13", NULL, 100, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    IVehicle* testCar = new Car("12R3", 50, 60);

    testRoad->enqueue(testCar);
    testRoad->updateVehicles();
    testRoad->checkAndReset();
    testRoad->updateVehicles();
    ASSERT_TRUE(testRoad->isEmpty());

    delete testRoad;
}


//TEST_F(RoadTester, RoadUpdate4)
//{
//    Road* testRoad = new Road("E13", NULL, 100, 50);
//    Car* testCar0 = new Car("12R3", 50, 30);
//    Car* testCar1 = new Car("AE-12", 40, 30);
//
//    testRoad->enqueue(testCar0);
//    testRoad->enqueue(testCar1);
//
//    testRoad->updateVehicles();
//    ASSERT_EQ(testCar0->getPosition(), 80);
//    ASSERT_EQ(testCar1->getPosition(), 70);
//
//    delete testRoad;
//}
//
//
//TEST_F(RoadTester, RoadUpdate5)
//{
//    testing::internal::CaptureStderr();
//
//    Road* testRoad = new Road("E13", NULL, 100, 50);
//    Car* testCar0 = new Car("12R3", 50, 10);
//    Car* testCar1 = new Car("AE-12", 40, 30);
//
//    testRoad->enqueue(testCar0);
//    testRoad->enqueue(testCar1);
//
//    testRoad->updateVehicles();
//    ASSERT_EQ(testCar0->getPosition(), 60);
//    ASSERT_EQ(testCar1->getPosition(), 70);
//
//    testing::internal::GetCapturedStderr();
//    delete testRoad;
//}
//
//TEST_F(RoadTester, RoadUpdate6)
//{
//    Road* testRoad = new Road("E13", NULL, 1000, 20);
//    Car* testCar = new Car("12R3", 0, 120);
//
//    testRoad->enqueue(testCar);
//    testRoad->updateVehicles();
//
//    ASSERT_EQ(testCar->getAcceleration(), testCar->getMinAcceleration());
//}
//
//TEST_F(RoadTester, RoadUpdate7)
//{
//
//    Road* testRoad0 = new Road("E13", NULL, 50, 150);
//    Road* testRoad1 = new Road("E13", testRoad0, 50, 150);
//
//    Car* testCar0 = new Car("12R3", 20, 70);
//
//    testRoad1->enqueue(testCar0);
//    testRoad1->updateVehicles();
//
//    ASSERT_EQ(testCar0->getPosition(), 40);
//    ASSERT_TRUE(testRoad1->isEmpty());
//    ASSERT_FALSE(testRoad0->isEmpty());
//
//    delete testRoad0;
//    delete testRoad1;
//}
//
//// Deze test beschrijft wat er gebeurt als een auto per tick meer dan 1 straat aflegt.
//TEST_F(RoadTester, RoadUpdate8)
//{
//
//    Road* testRoad0 = new Road("E13", NULL, 50, 150);
//    Road* testRoad1 = new Road("E13", testRoad0, 50, 150);
//
//    Car* testCar0 = new Car("12R3", 25, 150);
//
//    testRoad1->enqueue(testCar0);
//    testRoad1->updateVehicles();
//
//    ASSERT_TRUE(testRoad1->isEmpty());
//    ASSERT_TRUE(testRoad0->isEmpty());
//
//    delete testRoad0;
//    delete testRoad1;
//}
//
//// Deze test beschrijft wat er gebeurt als een auto enqueud wordt met hogere position dan het einde v/d straat.
//TEST_F(RoadTester, RoadUpdate9)
//{
//    Road* testRoad0 = new Road("E13", NULL, 50, 150);
//    Road* testRoad1 = new Road("E13", testRoad0, 50, 150);
//
//    Car* testCar0 = new Car("12R3", 20, 70);
//    Car* testCar1 = new Car("AE-12", 0, 60);
//
//    testRoad1->enqueue(testCar0);
//    testRoad1->enqueue(testCar1);
//
//    testRoad1->updateVehicles();
//
//    ASSERT_EQ(testCar0->getPosition(), 40);
//    ASSERT_EQ(testCar1->getPosition(), 10);
//
//    ASSERT_TRUE(testRoad1->isEmpty());
//    ASSERT_FALSE(testRoad0->isEmpty());
//
//    delete testRoad0;
//    delete testRoad1;
//}