#include "Car.h"

const double Car::fgkMaxAcceleration = 8.0;
const double Car::fgkMinAcceleration = -2.0;

const double Car::fgkMinSpeed = 0.0;
const double Car::fgkMaxSpeed = 150.0;

const double Car::fgkVehicleLength = 3;

void Car::move(const IVehicle* const next)
{
    fPosition += fVelocity;
    fVelocity += fAcceleration;

    double ideal  = 0.75 * fVelocity + next->getVehicleLength() + 2;
    double actual = next->getPosition() - next->getVehicleLength() - fPosition;

    fAcceleration = 0.5 * (actual - ideal);
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





