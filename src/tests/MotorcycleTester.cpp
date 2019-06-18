//============================================================================
// @name        : CarTester.cpp
// @author      : Thomas Dooms
// @date        : 19.03.19
// @version     : 1.0
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <gtest/gtest.h>
#include "../datatypes/vehicles/Motorcycle.h"

#include "../datatypes/Road.h"

class MotorcycleTester : public ::testing::Test
{
protected:
    virtual void SetUp(){}
    virtual void TearDown(){}
};

TEST_F(MotorcycleTester, move1)
{
    IVehicle* vehicle = new Motorcycle("AAA-123", 0, 0);
    const Zone* zone = new Zone(0, 100);
    Road road("E19", NULL, 5000, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    road.enqueue(vehicle);
    vehicle->move(0,0, &road);

    ASSERT_EQ(vehicle->getPosition(), vehicle->getMaxAcceleration());
    ASSERT_EQ(vehicle->getVelocity(), vehicle->getMaxAcceleration());
    ASSERT_EQ(vehicle->getAcceleration(), vehicle->getMaxAcceleration());
}

TEST_F(MotorcycleTester, move2)
{
    IVehicle* vehicle = new Motorcycle("AAA-123", 0, 29);
    const Zone* zone = new Zone(0, 30);
    Road road("E19", NULL, 5000, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    road.enqueue(vehicle);
    vehicle->move(0, 0, &road);

    ASSERT_EQ(vehicle->getPosition(), 30);
    ASSERT_EQ(vehicle->getVelocity(), 30);
    ASSERT_EQ(vehicle->getAcceleration(), 1);
}

TEST_F(MotorcycleTester, move3)
{
    IVehicle* vehicle = new Motorcycle("AAA-123", 0, 0);
    const Zone* zone = new Zone(0, 30);
    Road road("E19", NULL, 5000, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    road.enqueue(vehicle);
    vehicle->move(0, 0, &road);
    vehicle->move(0, 0, &road);
    vehicle->move(0, 0, &road);

    ASSERT_EQ(vehicle->getPosition(), 4);
    ASSERT_EQ(vehicle->getVelocity(), 4);
    ASSERT_EQ(vehicle->getAcceleration(), 4);
}
