//============================================================================
// @name        : TrafficSigns.cpp
// @author      : Thomas Dooms
// @date        : 4/4/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "TrafficSigns.h"
#include "Vehicles/IVehicle.h"

void TrafficLight::update() const { }
TrafficLight::Color TrafficLight::getColor() const { return green; }


void BusStop::update()
{
    stationed->setMoved(true);
}


bool operator<(const double a, const TrafficLight& b)
{
    return a < b.fPosition;
}

bool operator<(const double a, const BusStop& b)
{
    return a < b.fPosition;
}

bool operator<(const double a, const Zone& b)
{
    return a < b.fPosition;
}