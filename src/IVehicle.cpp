#include "IVehicle.h"
#include <iostream>
#include "tests/DesignByContract.h"

IVehicle::IVehicle(const std::string& license, double position, double velocity)
{
    _initCheck = this;
    fLicensePlate = license;

    fPosition = position;
    fVelocity = velocity;
    fAcceleration = 0;
}

bool IVehicle::properlyInitialized() const
{
    return _initCheck == this;
}

std::string IVehicle::getLicensePlate()
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
    return fLicensePlate;
}

double IVehicle::getPosition() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
    return fPosition;
}

double IVehicle::getVelocity() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getVelocity");
    return fVelocity;
}

double IVehicle::getAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
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
    return a.fPosition > b.fPosition;
}

void IVehicle::printVehicle(std::ostream& stream, const std::string& roadName) const
{
    const std::string toString[1] = {"auto"};
    stream << "Voertuig: " + toString[getType()] +'('+ fLicensePlate + ")\n";
    stream << "  -> Baan    : " << roadName  << '\n';
    stream << "  -> Positie : " << fPosition << '\n';
    stream << "  -> Snelheid: " << fVelocity << '\n';
}

