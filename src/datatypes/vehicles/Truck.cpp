//============================================================================
// @name        : Truck.cpp
// @author      : Thomas Dooms
// @date        : 4/3/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "Truck.h"
#include "../../DesignByContract.h"

const double Truck::fgkMaxAcceleration = 1.0;
const double Truck::fgkMinAcceleration = -6.0;

const double Truck::fgkMinSpeed = 0.0;
const double Truck::fgkMaxSpeed = 90.0 / 3.6;

const double Truck::fgkVehicleLength = 15;

Truck::Truck(const std::string& license, double position, double velocity) : IVehicle(license, position, velocity){}

double Truck::getVehicleLength() const
{
    REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getVehicleLength");
    return fgkVehicleLength;
}

std::string Truck::getType() const
{
    REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getType");
    return "vrachtwagen";
}

double Truck::getMaxSpeed() const
{
    REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getMaxSpeed");
    return fgkMaxSpeed;
}

double Truck::getMinSpeed() const
{
    REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getMinSpeed");
    return fgkMinSpeed;
}

double Truck::getMaxAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getMaxAcceleration");
    return fgkMaxAcceleration;
}

double Truck::getMinAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getMinAcceleration");
    return fgkMinAcceleration;
}







