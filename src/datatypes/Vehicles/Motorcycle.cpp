//============================================================================
// @name        : Motorcycle.cpp
// @author      : Thomas Dooms
// @date        : 4/3/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "Motorcycle.h"
#include "../../tests/DesignByContract.h"

const double Motorcycle::fgkMaxAcceleration = 4.0;
const double Motorcycle::fgkMinAcceleration = -10.0;

const double Motorcycle::fgkMinSpeed = 0.0;
const double Motorcycle::fgkMaxSpeed = 180.0 / 3.6;

const double Motorcycle::fgkVehicleLength = 1;

Motorcycle::Motorcycle(const std::string& license, double position, double velocity) : Vehicle(license, position, velocity){}

double Motorcycle::getVehicleLength() const
{
    REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getVehicleLength");
    return fgkVehicleLength;
}

std::string Motorcycle::getType() const
{
    REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getType");
    return "motorfiets";
}

double Motorcycle::getMaxSpeed() const
{
    REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getMaxSpeed");
    return fgkMaxSpeed;
}

double Motorcycle::getMinSpeed() const
{
    REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getMinSpeed");
    return fgkMinSpeed;
}

double Motorcycle::getMaxAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getMaxAcceleration");
    return fgkMaxAcceleration;
}

double Motorcycle::getMinAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getMinAcceleration");
    return fgkMinAcceleration;
}


