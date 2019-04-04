#include "IVehicle.h"
#include <iostream>
#include "../DesignByContract.h"
#include "../TrafficSigns.h"

double clamp(double val, double min, double max){ return std::max(std::min(val, max), min); }

const double IVehicle::fgkMinVehicleDist = 5.0;

IVehicle::IVehicle(const std::string& license, double position, double velocity)
{
    REQUIRE(velocity >= 0, "Velocity must be greater than 0");
    REQUIRE(position >= 0, "Position must be greater than 0");
    REQUIRE(!license.empty(), "License plate must be valid");

    _initCheck = this;
    fMoved = false;
    fLicensePlate = license;

    fPosition = position;
    fVelocity = velocity;
    fAcceleration = 0;

    ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
}

bool IVehicle::properlyInitialized() const
{
    return _initCheck == this;
}

void IVehicle::move(std::pair<const IVehicle*, double> nextVehicle, std::pair<const TrafficLight*, double> nextTrafficLight, std::pair<const BusStop*, double> nextBusStop, double speedLimit)
{
    REQUIRE(speedLimit > 0, "Speedlimit must be greater than 0");
    REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");
    REQUIRE(nextVehicle.first == NULL or nextVehicle.first->getPosition() + nextVehicle.second - this->getPosition() > fgkMinVehicleDist, "distance between vehicles must be greater than minVehicleDist");

    if(fMoved) return;
    if(nextVehicle.first != NULL and fPosition + fVelocity >= pairPosition<IVehicle>(nextVehicle) + nextVehicle.first->getVelocity() - nextVehicle.first->getVehicleLength())
        std::cerr << "car crash is imminent.\n";

    fPosition += fVelocity;                                                                 // Calculate new positions
    fVelocity += fAcceleration;                                                             // Calculate new velocity

    double kAcceleration = getFollowingAcceleration(nextVehicle);                           // calculate the following speed

    const std::pair<double, double> kMinMax = getMinMaxAcceleration(speedLimit);            // calculate the min and max acceleration possible
    const std::pair<bool  , double> kLight  = checkTrafficLights(nextTrafficLight);         // calculate the slowdown if needed
    const std::pair<bool  , double> kBus    = checkBusStop(nextBusStop);                    // calculate the slowdown if needed

    if(kLight.first) kAcceleration = std::min(kLight.second, kAcceleration);                // we need to take the minimum of these values
    if(kBus.first  ) kAcceleration = std::min(kBus.second  , kAcceleration);                // to ensure we slow down enough

    fAcceleration = clamp(kAcceleration, kMinMax.first, kMinMax.second);                    // clamp the values
    fMoved = true;

    ENSURE(getVelocity() >= 0, "Velocity cannot be negative");
    ENSURE((getAcceleration() >= getMinAcceleration()) && (getAcceleration() <= getMaxAcceleration()), "Acceleration is too high / low");
}

double IVehicle::getFollowingAcceleration(std::pair<const IVehicle*, double> nextVehicle) const
{
    if(nextVehicle.first == NULL) return getMaxAcceleration();                                                  // if there is not car in front, acceleration = max

    double ideal  = 0.75 * fVelocity + nextVehicle.first->getVehicleLength() + 2;                               // ideal following distance = 3/4 speed + 2 meters extra
    double actual = pairPosition<IVehicle>(nextVehicle) - nextVehicle.first->getVehicleLength() - fPosition;    // distance between 2 vehicles
    return 0.5 * (actual - ideal);                                                                              // take the average
}

std::pair<double, double> IVehicle::getMinMaxAcceleration(double speedlimit) const
{
    double maxSpeed = std::min(speedlimit, getMaxSpeed());                              // take the maxSpeed as the minimum of both
    double minSpeed = std::max(0.0       , getMinSpeed());                              // if the minimum speed is negative for some reason

    double maxAcceleration = maxSpeed - getVelocity();                                  // check if going to fast
    double minAcceleration = minSpeed - getVelocity();                                  // check if going too slow

    double clampedMax = clamp(maxAcceleration, getMinAcceleration(), getMaxAcceleration());
    double clampedMin = clamp(minAcceleration, getMinAcceleration(), getMaxAcceleration());

    return std::pair<double, double>(clampedMin, clampedMax);
}

std::pair<bool, double> IVehicle::checkTrafficLights(std::pair<const TrafficLight*, double> nextTrafficLight) const
{
    if(nextTrafficLight.first == NULL) return std::pair<bool, double>(false, 0);

    if(fPosition + fVelocity > pairPosition<TrafficLight>(nextTrafficLight) and nextTrafficLight.first->getColor() == TrafficLight::red) std::cerr<< "someone ran through a red light\n";

    double ideal = 0.75 * fVelocity;
    double dist = nextTrafficLight.first->getPosition() + nextTrafficLight.second - fPosition;

    if(dist < 2 * ideal and nextTrafficLight.first->getColor() != TrafficLight::green) return std::pair<bool, double>(true, -fVelocity*fVelocity / dist);
    else return std::pair<bool, double>(false, 0);
}

std::pair<bool, double> IVehicle::checkBusStop(std::pair<const BusStop*, double> nextBusStop) const
{
    if(nextBusStop.first == NULL or getType() != "bus") return std::pair<bool, double>(false, 0);

    if(fPosition + fVelocity == pairPosition<BusStop>(nextBusStop)) std::cerr << "a bus arrived\n";

    double dist = nextBusStop.first->getPosition() + nextBusStop.second - fPosition;

    if(dist < 100) return std::pair<bool, double>(true, -fVelocity*fVelocity / dist);
    else return std::pair<bool, double>(false, 0);
}

std::string IVehicle::getLicensePlate() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
    return fLicensePlate;
}

double IVehicle::getPosition() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
    return fPosition;
}
void IVehicle::setPosition(double position)
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setPosition");
    fPosition = position;
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

void IVehicle::setMoved(bool moved)
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMoved");
    fMoved = moved;
}

bool operator<(const IVehicle& a, const IVehicle& b)
{
    REQUIRE(a.properlyInitialized() && b.properlyInitialized(), "one of the Vehicles was not initialized when calling operator<");
    return a.fPosition > b.fPosition;
}

