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

const uint32_t TrafficLight::fgkMaxDifference = 100;

TrafficLight::TrafficLight(const double kPosition)
{
    fPosition = kPosition;
}
void TrafficLight::update()
{
    if(fInRange->getPosition() >= fPosition) fInRange = NULL;

    const uint32_t diff = fRedTime - fGreenTime;
    switch(fColor)
    {
        case red:
            // if there is a vehicle incoming or the redTime has been too long, switch to green after 15 ticks
            if((fInRange != NULL or diff > fgkMaxDifference) and fTimer >= 15)
            {
                fColor = green;
                fTimer = 0;
            }
            break;

        case orange:
            // switch to red after 5 ticks
            if(fTimer >= 5)
            {
                fColor = red;
                fTimer = 0;
            }
            break;

        case green:
            // if there is no vehicle incoming or greenTime has been too high, switch to orange after 15 ticks
            if((fInRange == NULL or diff < -fgkMaxDifference) and fTimer >= 15)
            {
                fColor = orange;
                fTimer = 0;
            }
            break;

        default:
            throw std::runtime_error("unknow trafficlight color");
    }

    fTimer++;
}
TrafficLight::Color TrafficLight::getColor() const
{
    return green;
}
void TrafficLight::setInRange(const IVehicle* vehicle)
{
    fInRange = vehicle;
}
double TrafficLight::getPosition() const
{
    return fPosition;
}

//--------------------------------------------------------------------------------------------------//

const uint32_t BusStop::stationTime = 30;

BusStop::BusStop(double kPosition)
{
    fPosition = kPosition;
}

void BusStop::update()
{
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
void BusStop::setStationed(IVehicle* vehicle)
{
    fStationed = vehicle;
}
double BusStop::getPosition() const
{
    return fPosition;
}

//--------------------------------------------------------------------------------------------------//

Zone::Zone(const double kPosition, const double kSpeedLimit)
{
    fPosition = kPosition;
    fSpeedlimit = kSpeedLimit;
}

double Zone::getSpeedlimit() const
{
    return fSpeedlimit;
}
double Zone::getPosition() const
{
    return fPosition;
}

//--------------------------------------------------------------------------------------------------//

bool operator<(const double lhs, const TrafficLight& rhs)
{
    return lhs < rhs.fPosition;
}

bool operator<(const double lhs, const BusStop& rhs)
{
    return lhs < rhs.fPosition;
}

bool operator<(const double lhs, const Zone& rhs)
{
    return lhs < rhs.fPosition;
}