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

TEST_F(RoadTester, RoadUpdate4)
{
    const Zone* zone = new Zone(0, 50);
    const Zone* zone2 = new Zone(100, 120);

    std::vector<const Zone*> zones;
    zones.emplace_back(zone);
    zones.emplace_back(zone2);

    Road* testRoad = new Road("E13", NULL, 150, 1, zones, std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    IVehicle* testCar = new Car("12R3", 0, 130);

    testRoad->enqueue(testCar);
    testRoad->updateVehicles();
    testRoad->checkAndReset();
    ASSERT_FALSE(testRoad->isEmpty());
    testRoad->updateVehicles();
    ASSERT_TRUE(testRoad->isEmpty());


    delete testRoad;
}

TEST_F(RoadTester, RoadUpdate5)
{
    const Zone* zone = new Zone(0, 70);
    Road* testRoad = new Road("E13", NULL, 150, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    IVehicle* testCar = new Car("12R3", 0, 69);

    testRoad->enqueue(testCar);
    testRoad->updateVehicles();
    ASSERT_FALSE(testRoad->isEmpty());
    testRoad->checkAndReset();
    testRoad->updateVehicles();
    ASSERT_FALSE(testRoad->isEmpty());
    testRoad->checkAndReset();
    testRoad->updateVehicles();
    ASSERT_TRUE(testRoad->isEmpty());


    delete testRoad;
}


TEST_F(RoadTester, RoadUpdate6)
{
    const Zone* zone = new Zone(0, 60);
    Road* testRoad = new Road("E13", NULL, 121, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    IVehicle* testCar = new Car("12R3", 0, 60);

    testRoad->enqueue(testCar);

    testRoad->updateVehicles();
    ASSERT_FALSE(testRoad->isEmpty());
    testRoad->checkAndReset();
    testRoad->updateVehicles();
    ASSERT_FALSE(testRoad->isEmpty());
    testRoad->checkAndReset();
    testRoad->updateVehicles();
    ASSERT_TRUE(testRoad->isEmpty());

    delete testRoad;
}

// auto die meer dan 2 straten aflegt
TEST_F(RoadTester, RoadUpdate7)
{
    const Zone* zone = new Zone(0, 120);
    Road* testRoad = new Road("E13", NULL, 120, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    [[maybe_unused]] Road* testRoad2 = new Road("E15", testRoad, 10, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    [[maybe_unused]] Road* testRoad3 = new Road("E13", testRoad2, 100, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());

    IVehicle* testCar1 = new Car("AE-12", 40, 120);

    testRoad3->enqueue(testCar1);
    testRoad3->updateVehicles();

    ASSERT_TRUE(testRoad3->isEmpty());
    ASSERT_TRUE(testRoad2->isEmpty());
    ASSERT_FALSE(testRoad->isEmpty());

    delete testRoad;
    delete testRoad2;
    delete testRoad3;
}


TEST_F(RoadTester, RoadUpdate8)
{
    const Zone* zone = new Zone(0, 50);
    Road* testRoad = new Road("E13", NULL, 100, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());

    Car* testCar0 = new Car("12R3", 50, 10);
    Car* testCar1 = new Car("AE-12", 0, 30);

    testRoad->enqueue(testCar0);
    testRoad->enqueue(testCar1);

    testRoad->updateVehicles();
    ASSERT_EQ(testCar0->getPosition(), 60);
    ASSERT_EQ(testCar1->getPosition(), 30);

    delete testRoad;
}


TEST_F(RoadTester, RoadUpdate9)
{
    const Zone* zone = new Zone(0, 20);
    Road* testRoad = new Road("E13", NULL, 1000, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());

    Car* testCar = new Car("12R3", 0, 120);

    testRoad->enqueue(testCar);
    testRoad->updateVehicles();

    ASSERT_EQ(testCar->getAcceleration(), testCar->getMinAcceleration());
}

TEST_F(RoadTester, RoadUpdate10)
{

    const Zone* zone = new Zone(0, 90);
    Road* testRoad0 = new Road("E13", NULL, 100, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());

    const Zone* zone2 = new Zone(0, 200);
    Road* testRoad1 = new Road("E15", testRoad0, 90, 1, std::vector<const Zone*>(1, zone2), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());

    Car* testCar0 = new Car("12R3", 20, 80);

    testRoad1->enqueue(testCar0);
    testRoad1->updateVehicles();

    ASSERT_EQ(testCar0->getPosition(), 10);
    ASSERT_TRUE(testRoad1->isEmpty());
    ASSERT_FALSE(testRoad0->isEmpty());

    delete testRoad0;
    delete testRoad1;
}

// test beschrijft wat er gebeurt als een auto per tick meer dan 1 straat aflegt.
TEST_F(RoadTester, RoadUpdate11)
{

    const Zone* zone = new Zone(0, 200);
    Road* testRoad0 = new Road("E13", NULL, 100, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());

    const Zone* zone2 = new Zone(0, 200);
    Road* testRoad1 = new Road("E15", testRoad0, 90, 1, std::vector<const Zone*>(1, zone2), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());


    Car* testCar0 = new Car("12R3", 25, 200);

    testRoad1->enqueue(testCar0);
    testRoad1->updateVehicles();

    ASSERT_TRUE(testRoad1->isEmpty());
    ASSERT_TRUE(testRoad0->isEmpty());

    delete testRoad0;
    delete testRoad1;
}

TEST_F(RoadTester, RoadUpdate12)
{
    const Zone* zone = new Zone(0, 160);
    Road* testRoad0 = new Road("E13", NULL, 40, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());

    const Zone* zone2 = new Zone(0, 160);
    Road* testRoad1 = new Road("E15", testRoad0, 40, 1, std::vector<const Zone*>(1, zone2), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());


    Car* testCar0 = new Car("12R3", 20, 70);
    Car* testCar1 = new Car("AE-12", 0, 60);

    testRoad1->enqueue(testCar0);
    testRoad1->enqueue(testCar1);

    testRoad1->updateVehicles();

    ASSERT_EQ(testCar0->getPosition(), 50);
    ASSERT_EQ(testCar1->getPosition(), 20);

    ASSERT_TRUE(testRoad1->isEmpty());
    ASSERT_FALSE(testRoad0->isEmpty());

    delete testRoad0;
    delete testRoad1;
}