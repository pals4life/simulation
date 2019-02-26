#include "IVehicle.h"

IVehicle::IVehicle(const std::string& license, double position)
{
    kfLicensePlate = license;

    fPosition = position;
    fVelocity = 0;
    fAcceleration = 0;
}

double IVehicle::getPosition() const
{
    return fPosition;
}

double IVehicle::getVelocity() const
{
    return fVelocity;
}

double IVehicle::getAcceleration() const
{
    return fAcceleration;
}

double& IVehicle::getPosition()
{
    return fPosition;
}

double& IVehicle::getVelocity()
{
    return fVelocity;
}

double& IVehicle::getAcceleration()
{
    return fAcceleration;
}