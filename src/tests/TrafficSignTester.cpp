//============================================================================
// @name        : TrafficSignTester.cpp
// @author      : Thomas Dooms
// @date        : 5/9/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <gtest/gtest.h>
#include "../datatypes/TrafficSigns.h"
#include "../datatypes/vehicles/Bus.h"

class TrafficSignTester : public ::testing::Test
{
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

};

TEST_F(TrafficSignTester, TrafficLightInit)
{
    EXPECT_DEATH(TrafficLight light(-1), "kPosition must be greater than 0");
}

TEST_F(TrafficSignTester, TrafficLightUpdate1)
{
    TrafficLight light(0);
    EXPECT_EQ(light.getColor(), TrafficLight::EColor::kGreen);
    for(uint32_t i = 0; i < 15; i++) light.update();
    EXPECT_EQ(light.getColor(), TrafficLight::EColor::kGreen);
}

TEST_F(TrafficSignTester, TrafficLightUpdate2)
{
    TrafficLight light(0);
    light.setColor(TrafficLight::EColor::kOrange);
    EXPECT_EQ(light.getColor(), TrafficLight::EColor::kOrange);
    for(uint32_t i = 0; i < 5; i++) light.update();
    EXPECT_EQ(light.getColor(), TrafficLight::EColor::kOrange);
    for(uint32_t i = 0; i < 5; i++) light.update();
    EXPECT_EQ(light.getColor(), TrafficLight::EColor::kRed);
}

TEST_F(TrafficSignTester, TrafficLightUpdate3)
{
    const IVehicle* bus = new Bus("aa", 0, 0);
    TrafficLight light(100);
    light.setColor(TrafficLight::EColor::kRed);
    for(uint32_t i = 0; i < 15; i++) light.update();
    light.setInRange(bus);
    light.update();
    EXPECT_EQ(light.getColor(), TrafficLight::EColor::kGreen);
}

TEST_F(TrafficSignTester, TrafficLightUpdate4)
{
    const IVehicle* bus = new Bus("aa", 100, 0);
    TrafficLight light(0);
    light.setColor(TrafficLight::EColor::kRed);
    for(uint32_t i = 0; i < 15; i++) light.update();
    light.setInRange(bus);
    light.update();
    EXPECT_EQ(light.getColor(), TrafficLight::EColor::kRed);
}

TEST_F(TrafficSignTester, BusStopInit)
{
    EXPECT_DEATH(BusStop stop(-1), "kPosition must be greater than 0");
}

TEST_F(TrafficSignTester, BusStopStationed)
{
    const IVehicle* bus = new Bus("aa", 0, 0);
    BusStop stop(0);

    stop.setStationed(bus);
    stop.update();
    EXPECT_TRUE(bus->getStationed());
}

TEST_F(TrafficSignTester, BusStopUpdate)
{
    const IVehicle* bus = new Bus("aa", 0, 0);
    BusStop stop(0);

    stop.setStationed(bus);
    stop.update();
    EXPECT_TRUE(bus->getStationed());
    for(uint32_t i = 0; i < 30; i++) stop.update();
    EXPECT_FALSE(bus->getStationed());
}

TEST_F(TrafficSignTester, ZoneInit)
{
    EXPECT_DEATH(Zone zone(-1, 0), "kPosition must be greater than 0"  );
    EXPECT_DEATH(Zone zone(0, -1), "kSpeedLimit must be greater than 0");
}
