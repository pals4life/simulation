#include "Car.h"
#include "../../DesignByContract.h"

const double Car::fgkMaxAcceleration = 2.0;
const double Car::fgkMinAcceleration = -8.0;

const double Car::fgkMinSpeed = 0.0;
const double Car::fgkMaxSpeed = 150.0 / 3.6;

const double Car::fgkVehicleLength = 3;

Car::Car(const std::string& license, double position, double velocity) : IVehicle(license, position, velocity){}

double Car::getVehicleLength() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getVehicleLength");
    return fgkVehicleLength;
}

std::string Car::getType() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getType");
    return "auto";
}

double Car::getMaxSpeed() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMaxSpeed");
    return fgkMaxSpeed;
}

double Car::getMinSpeed() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMinSpeed");
    return fgkMinSpeed;
}

double Car::getMaxAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMaxAcceleration");
    return fgkMaxAcceleration;
}

double Car::getMinAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMinAcceleration");
    return fgkMinAcceleration;
}







