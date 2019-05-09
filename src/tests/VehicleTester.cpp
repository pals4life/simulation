//============================================================================
// @name        : CarTester.cpp
// @author      : Thomas Dooms
// @date        : 19.03.19
// @version     : 1.0
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <gtest/gtest.h>
#include "../datatypes/vehicles/Car.h"
#include "../datatypes/vehicles/Truck.h"

#include "../datatypes/Road.h"

class VehicleTester : public ::testing::Test
{
protected:
    virtual void SetUp(){}
    virtual void TearDown(){}
};

TEST_F(VehicleTester, constructor)
{
    EXPECT_DEATH(Car car(""       ,  0,  0), "License plate must be valid"    );
    EXPECT_DEATH(Car car("AAA-123", -1,  0), "Position must be greater than 0");
    EXPECT_DEATH(Car car("AAA-123",  0, -1), "Velocity must be greater than 0");
}

TEST_F(VehicleTester, move1)
{
    IVehicle* car = new Car("AAA-123", 0, 0);
    const Zone* zone = new Zone(0, 100);
    Road road("E19", NULL, 5000, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    road.enqueue(car);
    car->move(0,0, &road);

    ASSERT_EQ(car->getPosition(), car->getMaxAcceleration());
    ASSERT_EQ(car->getVelocity(), car->getMaxAcceleration());
    ASSERT_EQ(car->getAcceleration(), car->getMaxAcceleration());
}

TEST_F(VehicleTester, move2)
{
    IVehicle* car = new Car("AAA-123", 0, 29);
    const Zone* zone = new Zone(0, 30);
    Road road("E19", NULL, 5000, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    road.enqueue(car);
    car->move(0, 0, &road);

    ASSERT_EQ(car->getPosition(), 30);
    ASSERT_EQ(car->getVelocity(), 30);
    ASSERT_EQ(car->getAcceleration(), 1);
}

TEST_F(VehicleTester, move3)
{
    IVehicle* car1 = new Car("AAA-123", 100, 0);
    IVehicle* bus1 = new Car("BBB-123", 99 + car1->getMinVehicleDist(), 0);
    IVehicle* car3 = new Car("BBB-123", 200, 0);

    const Zone* zone = new Zone(0, 100);
    Road road("E19", NULL, 5000, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    road.enqueue(car3);
    road.enqueue(bus1);
    road.enqueue(car1);

    EXPECT_DEATH(car1->move(0, 2, &road), "distance between vehicles must be greater than minVehicleDist");
}

TEST_F(VehicleTester, truckMove1)
{
    IVehicle* vehicle = new Truck("AAA-123", 0, 10);
    const Zone* zone = new Zone(0, 30);
    Road road("E19", NULL, 5000, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>());
    road.enqueue(vehicle);
    vehicle->move(0, 0, &road);

    ASSERT_EQ(vehicle->getPosition(), 10 + vehicle->getMaxAcceleration());
    ASSERT_EQ(vehicle->getVelocity(), 10 + vehicle->getMaxAcceleration());
    ASSERT_EQ(vehicle->getAcceleration(), vehicle->getMaxAcceleration());
}
