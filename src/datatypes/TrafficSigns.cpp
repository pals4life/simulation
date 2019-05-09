//============================================================================
// @name        : TrafficSigns.cpp
// @author      : Thomas Dooms
// @date        : 4/4/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <stdexcept>
#include <iostream>
#include "TrafficSigns.h"
#include "vehicles/IVehicle.h"
#include "../DesignByContract.h"

const uint32_t TrafficLight::fgkMaxDifference = 100;

TrafficLight::TrafficLight(const double kPosition)
{
    REQUIRE(kPosition > 0, "kPosition must be greater than 0");

    fPosition = kPosition;

    fColor = kGreen;
    fkInRange = NULL;

    fTimer = 0;
    fRedTime = 0;
    fGreenTime = 0;

    _initCheck = this;

    ENSURE(properlyInitialized(), "TrafficLight constructor must end in properly initialized state");
}

bool TrafficLight::properlyInitialized() const
{
    return _initCheck == this;
}

void TrafficLight::update() const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling updateVehicles");

    if(fkInRange != NULL and fkInRange->getPosition() >= fPosition) fkInRange = NULL;

    const uint32_t diff = fRedTime - fGreenTime;
    switch(fColor)
    {
        case kRed:
            // if there is a vehicle incoming or the redTime has been too long, switch to kGreen after 15 ticks
            if((fkInRange != NULL or diff > fgkMaxDifference) and fTimer >= 15)
            {
                fColor = kGreen;
                fTimer = 0;
            }
            fRedTime++;
            break;

        case kOrange:
            // switch to kRed after 5 ticks
            if(fTimer >= 5)
            {
                fColor = kRed;
                fTimer = 0;
            }
            break;

        case kGreen:
            // if there is no vehicle incoming or greenTime has been too high, switch to kOrange after 15 ticks
            if((fkInRange == NULL or diff < -fgkMaxDifference) and fTimer >= 15)
            {
                fColor = kOrange;
                fTimer = 0;
            }
            fGreenTime++;
            break;

        default:
            throw std::runtime_error("unknown traffic light color");
    }

    fTimer++;
}
TrafficLight::EColor TrafficLight::getColor() const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getColor");
    return fColor;
}
void TrafficLight::setColor(const EColor color) const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getColor");
    fColor = color;
    ENSURE(getColor() == color, "new color not set when calling setColor");
}

const IVehicle* TrafficLight::getInRange() const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getInRange");
    return fkInRange;
}
void TrafficLight::setInRange(const IVehicle* const kVehicle) const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling setInRange");
    REQUIRE(kVehicle->properlyInitialized(), "kVehicles must be properly initialized");
    fkInRange = kVehicle;
    ENSURE(getInRange() == kVehicle, "new in range vehicle not set when calling setInRange");
}
double TrafficLight::getPosition() const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getPosition");
    return fPosition;
}

//--------------------------------------------------------------------------------------------------//

const uint32_t BusStop::fgkStationTime = 30;

BusStop::BusStop(double kPosition)
{
    REQUIRE(kPosition > 0, "kPosition must be greater than 0");
    fPosition = kPosition;

    fStationed = NULL;
    fTimer = 0;

    _initCheck = this;
    ENSURE(properlyInitialized(), "BusStop constructor must end in properly initialized state");
}

bool BusStop::properlyInitialized() const
{
    return _initCheck == this;
}

void BusStop::update() const
{
    REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling update");
    if(fStationed != NULL)
    {
        fStationed->setStationed(true);
        fTimer++;
    }
    if(fTimer == fgkStationTime)
    {
        fStationed->setStationed(false);
        fStationed = NULL;
        fTimer = 0;
    }
    ENSURE(fTimer < fgkStationTime, "Bus stationed for too long");
}

const IVehicle* BusStop::getStationed() const
{
    REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling getStationed");
    return fStationed;
}

void BusStop::setStationed(const IVehicle* const kVehicle) const
{
    REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling setStationed");
    REQUIRE(kVehicle->properlyInitialized(), "kVehicles must be properly initialized");
    fStationed = kVehicle;
    ENSURE(getStationed() == kVehicle, "new stationed vehicle not set when calling setStationed");
}

double BusStop::getPosition() const
{
    REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling getPosition");
    return fPosition;
}

//--------------------------------------------------------------------------------------------------//

Zone::Zone(const double kPosition, const double kSpeedLimit)
{
    REQUIRE(kPosition   >= 0, "kPosition must be greater than 0"  );
    REQUIRE(kSpeedLimit >= 0, "kSpeedLimit must be greater than 0");
    fPosition = kPosition;
    fSpeedlimit = kSpeedLimit;
    _initCheck = this;
    ENSURE(properlyInitialized(), "Zone constructor must end in properly initialized state");
}

bool Zone::properlyInitialized() const
{
    return _initCheck == this;
}

double Zone::getSpeedlimit() const
{
    REQUIRE(properlyInitialized(), "Zone was not properly initialized when calling getSpeedlimit");
    return fSpeedlimit;
}

void Zone::setSpeedLimit(const double kSpeedlimit) const
{
    REQUIRE(properlyInitialized(), "Zone was not properly initialized when calling setSpeedLimit");
    REQUIRE(kSpeedlimit >= 0, "kSpeedLimit must be greater than 0");
    fSpeedlimit = kSpeedlimit;
    ENSURE(getSpeedlimit() == kSpeedlimit, "new speedlimit not set when calling setSpeedLimit");
}

double Zone::getPosition() const
{
    REQUIRE(properlyInitialized(), "Zone was not properly initialized when calling setSpeedLimit");
    return fPosition;
}

