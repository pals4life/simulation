//============================================================================
// @name        : Bus.cpp
// @author      : Thomas Dooms
// @date        : 4/3/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "Bus.h"
#include "../DesignByContract.h"

const double Bus::fgkMaxAcceleration = 1.0;
const double Bus::fgkMinAcceleration = -7.0;

const double Bus::fgkMinSpeed = 0.0;
const double Bus::fgkMaxSpeed = 70.0 / 3.6;

const double Bus::fgkVehicleLength = 10;

Bus::Bus(const std::string& license, double position, double velocity) : IVehicle(license, position, velocity){}

double Bus::getVehicleLength() const
{
    REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getVehicleLength");
    return fgkVehicleLength;
}

std::string Bus::getType() const
{
    REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getType");
    return "bus";
}

double Bus::getMaxSpeed() const
{
    REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getMaxSpeed");
    return fgkMaxSpeed;
}

double Bus::getMinSpeed() const
{
    REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getMinSpeed");
    return fgkMinSpeed;
}

double Bus::getMaxAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getMaxAcceleration");
    return fgkMaxAcceleration;
}

double Bus::getMinAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getMinAcceleration");
    return fgkMinAcceleration;
}







