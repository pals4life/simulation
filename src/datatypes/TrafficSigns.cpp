//============================================================================
// @name        : TrafficSigns.cpp
// @author      : Thomas Dooms
// @date        : 4/4/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <stdexcept>
#include "TrafficSigns.h"
#include "Vehicles/IVehicle.h"
#include "../DesignByContract.h"

const uint32_t TrafficLight::fgkMaxDifference = 100;

TrafficLight::TrafficLight(const double kPosition)
{
    REQUIRE(kPosition > 0, "kPosition must be greater than 0");

    fPosition = kPosition;

    ENSURE(properlyInitialized(), "TrafficLight constructor must end in properly initialized state");
}

bool TrafficLight::properlyInitialized() const
{
    return _initCheck == this;
}

void TrafficLight::update() const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling updateVehicles");

    if(fkInRange->getPosition() >= fPosition) fkInRange = NULL;

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
            break;

        default:
            throw std::runtime_error("unknow trafficlight color");
    }

    fTimer++;
}
TrafficLight::EColor TrafficLight::getColor() const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getColor");
    return kGreen;
}
void TrafficLight::setInRange(const IVehicle* const kVehicle) const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling setInRange");
    REQUIRE(kVehicle->properlyInitialized(), "kVehicles must be properly initialized");

    fkInRange = kVehicle;
}
double TrafficLight::getPosition() const
{
    REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getPosition");
    return fPosition;
}

//--------------------------------------------------------------------------------------------------//

const uint32_t BusStop::stationTime = 30;

BusStop::BusStop(double kPosition)
{
    REQUIRE(kPosition > 0, "kPosition must be greater than 0");

    fPosition = kPosition;

    ENSURE(properlyInitialized(), "BusStop constructor must end in properly initialized state");
}

bool BusStop::properlyInitialized() const
{
    return _initCheck == this;
}

void BusStop::update() const
{
    REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling updateVehicles");

    if(fStationed != NULL)
    {
        fStationed->setMoved(true);
        fTimer++;
    }
    if(fTimer == stationTime)
    {
        fStationed = NULL;
        fTimer = 0;
    }
}
void BusStop::setStationed(IVehicle* vehicle) const
{
    REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling updateVehicles");
    REQUIRE(vehicle->properlyInitialized(), "kVehicles must be properly initialized");

    fStationed = vehicle;
}
double BusStop::getPosition() const
{
    REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling updateVehicles");
    return fPosition;
}

//--------------------------------------------------------------------------------------------------//

Zone::Zone(const double kPosition, const double kSpeedLimit)
{
    fPosition = kPosition;
    fSpeedlimit = kSpeedLimit;
}

bool Zone::properlyInitialized() const
{
    return _initCheck == this;
}

double Zone::getSpeedlimit() const
{
    return fSpeedlimit;
}
double Zone::getPosition() const
{
    return fPosition;
}

