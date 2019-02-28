#include "IVehicle.h"
#include <iostream>

IVehicle::IVehicle(const std::string& license, double position, double velocity)
{
    fLicensePlate = license;

    fPosition = position;
    fVelocity = velocity;
    fAcceleration = 0;
}

std::string IVehicle::getLicensePlate()
{
    return fLicensePlate;
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

bool operator<(const IVehicle& a, const IVehicle& b)
{
    return a.fPosition < b.fPosition;
}

void IVehicle::printVehicle(std::ostream& stream, const std::string& roadName) const
{
    const std::string toString[1] = {"auto"};
    stream << "Voertuig: " + toString[getType()] +'('+ fLicensePlate + ")\n";
    stream << "  -> Baan    : " << roadName  << '\n';
    stream << "  -> Positie : " << fPosition << '\n';
    stream << "  -> Snelheid: " << fVelocity << '\n';
}

