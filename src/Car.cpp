#include "Car.h"

const double Car::fgkMaxAcceleration = 8.0;
const double Car::fgkMinAcceleration = -2.0;

const double Car::fgkMinSpeed = 0.0;
const double Car::fgkMaxSpeed = 150.0;

const double Car::fgkVehicleLength = 3;

Car::Car(const std::string& license, double pos, double velocity) : IVehicle(license, pos, velocity) {}

void Car::move(const IVehicle* const next, double speedLimit)
{
    fPosition += fVelocity;
    fVelocity += fAcceleration;

    if(next == NULL)
    {
        fAcceleration = std::min(fgkMaxAcceleration, speedLimit - fVelocity);
    }
    else
    {
        double ideal  = 0.75 * fVelocity + next->getVehicleLength() + 2;
        double actual = next->getPosition() - next->getVehicleLength() - fPosition;

        double minAcceleration = std::max(fVelocity, fgkMinAcceleration);               // make sure it doesn't slow down below 0
        double maxAcceleration = std::min(speedLimit - fVelocity, fgkMaxAcceleration);  // make sure it doesn't accelerate higher than speed limit

        fAcceleration = 0.5 * (actual - ideal);
        fAcceleration = std::min(std::max(fAcceleration, minAcceleration), maxAcceleration); // clamp the value
    }
}

double Car::getVehicleLength() const
{
    return fgkVehicleLength;
}

EVehicleType Car::getType() const
{
    return kCar;
}

double Car::getMaxAcceleration() const
{
    return fgkMaxAcceleration;
}

double Car::getMinAcceleration() const
{
    return fgkMinAcceleration;
}

double Car::getMinVelocity() const
{
    return fgkMinSpeed;
}

double Car::getMaxVelocity() const
{
    return fgkMaxSpeed;
}





