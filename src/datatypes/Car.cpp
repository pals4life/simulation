#include <iostream>
#include "Car.h"
#include "../tests/DesignByContract.h"

const double Car::fgkMaxAcceleration = 2.0;
const double Car::fgkMinAcceleration = -8.0;

const double Car::fgkMinSpeed = 0.0;
const double Car::fgkMaxSpeed = 150.0;

const double Car::fgkVehicleLength = 3;

Car::Car(const std::string& license, double position, double velocity) : IVehicle(license, position, velocity){}

void Car::move(const IVehicle* const next, double speedLimit, double offset)
{
    REQUIRE(speedLimit > 0, "Speedlimit must be greater than 0");
    REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");
    if(next != NULL) REQUIRE(next->getPosition() + offset - this->getPosition() > fgkMinVehicleDist, "distance between vehicles must be greater than minVehicleDist");
    if(next != NULL and fPosition + fVelocity >= offset + next->getPosition() + next->getVelocity() - next->getVehicleLength()) std::cerr << "car crash is imminent.\n";
    if(moved) return;

    fPosition += fVelocity;                                                                 // Calculate new positions
    fVelocity += fAcceleration;                                                             // Calculate new velocity
                                                                                            // Calculate new acceleration in following code
    if(next == NULL)
    {
        fAcceleration = fgkMaxAcceleration;                                                 // if nobody is ahead, go max acceleration
    }
    else
    {
        double ideal  = 0.75 * fVelocity + next->getVehicleLength() + 2;                    // ideal following distance = 3/4 speed + 2 meters extra
        double actual = offset+next->getPosition() - next->getVehicleLength() - fPosition;  // distance between 2 vehicles
        fAcceleration = 0.5 * (actual - ideal);                                             // take the average
    }

    if(fVelocity > speedLimit) fAcceleration = fVelocity - speedLimit;                      // if above speed limit, slow down (occurs when changing roads)
    double minAcceleration = std::max(-fVelocity, fgkMinAcceleration);                      // determine min acceleration, to prevent negative speed
    double maxAcceleration = std::min(speedLimit - fVelocity, fgkMaxAcceleration);          // determine max acceleration, to prevent from going too fast

    minAcceleration = std::min(minAcceleration, fgkMaxAcceleration);                        // make sure the resulting min acceleration is smaller than the max acceleration
    maxAcceleration = std::max(maxAcceleration, fgkMinAcceleration);                        // make sure the resulting max acceleration is greater than the min acceleration

    fAcceleration = std::min(std::max(fAcceleration, minAcceleration), maxAcceleration);    // clamp the acceleration
    moved = true;

    ENSURE(fVelocity >= 0, "Velocity cannot be negative");
    ENSURE((fAcceleration >= fgkMinAcceleration) && (fAcceleration <= fgkMaxAcceleration), "Acceleration is too high / low");
}

double Car::getVehicleLength() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getVehicleLength");
    return fgkVehicleLength;
}

EVehicleType Car::getType() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getType");
    return kCar;
}

double Car::getMaxVelocity() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMaxVelocity");
    return fgkMaxSpeed;
}

double Car::getMinVelocity() const
{
    REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMinVelocity");
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







