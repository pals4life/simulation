#include "IVehicle.h"
#include <iostream>
#include "../../DesignByContract.h"
#include "../../exporters/VehicleExporter.h"
#include "../TrafficSigns.h"
#include "../Road.h"
#include "../util.h"

double clamp(double val, double min, double max){ return std::max(std::min(val, max), min); }

const double IVehicle::fgkMinVehicleDist = 5.0;
const double IVehicle::fgkEpsilonThreshold = 0.01;

IVehicle::IVehicle(const std::string& license, double position, double velocity)
{
    REQUIRE(velocity >= 0, "Velocity must be greater than 0");
    REQUIRE(position >= 0, "Position must be greater than 0");
    REQUIRE(!license.empty(), "License plate must be valid");

    _initCheck = this;

    fMoved = false;
    fStationed = false;

    fLicensePlate = license;

    fPosition = position;
    fVelocity = velocity;
    fAcceleration = 0;

    fPrevAcceleration = std::deque<double>(5, 0);

    fTrafficLightAccel = std::pair<bool, double>(false, 0);
    fBusStopAccel = std::pair<bool, double>(false, 0);

    fTimer = 0;
    fDriveTimer = 0;
    fDistance = 0;
    fMaxVelocity = 0;

    ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
}
IVehicle::~IVehicle()
{
    VehicleExporter::addSection(this);
}

bool IVehicle::properlyInitialized() const
{
    return _initCheck == this;
}

void IVehicle::move(const uint32_t lane, const uint32_t index, Road* road)
{
    REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");

    if(fMoved) return;      // moved means the vehicle already has been updated
    updateStatistics();
    if(fStationed) return;  // stationed means the vehicle must not update

    double oldPosition = fPosition;
    fPosition += fVelocity;                                                                         // Calculate new positions
    fVelocity += fAcceleration;                                                                     // Calculate new velocity

    const std::pair<const IVehicle*, double> nextVehicle = road->getNextVehicle(lane, index);       // get the next vehicle
    double kAcceleration = getFollowingAcceleration(nextVehicle);                                   // calculate the following speed

    const std::pair<double, double> kMinMax = getMinMaxAcceleration(road->getSpeedLimit(oldPosition));// calculate the min and max acceleration possible

    checkTrafficLights(road->getTrafficLight(oldPosition));                                          // calculate the slowdown if needed
    checkBusStop(road->getBusStop(oldPosition));                                                     // calculate the slowdown if needed

    if(fTrafficLightAccel.first) kAcceleration = std::min(fTrafficLightAccel.second, kAcceleration); // we need to take the minimum of these values
    if(fBusStopAccel.first     ) kAcceleration = std::min(fBusStopAccel.second     , kAcceleration); // to ensure we slow down enough

    fAcceleration = clamp(kAcceleration, kMinMax.first, kMinMax.second);                            // clamp the values
    fMoved = true;

    fPrevAcceleration.push_front(fAcceleration);
    fPrevAcceleration.pop_back();

    if(road->laneExists(lane+1)) checkLaneChange(fTrafficLightAccel.first, lane, index, road, true );// overtake if possible
    if(road->laneExists(lane-1)) checkLaneChange(fTrafficLightAccel.first, lane, index, road, false);// go back if possible

    ENSURE(getVelocity() >= 0, "Velocity cannot be negative");
    ENSURE((getAcceleration() >= getMinAcceleration()) && (getAcceleration() <= getMaxAcceleration()), "Acceleration is too high / low");
//    ENSURE(nextVehicle.first == NULL or pairPosition<IVehicle>(nextVehicle) - getPosition() > getMinVehicleDist(), "distance between vehicles must be greater than minVehicleDist");
    ENSURE(fPrevAcceleration.size() == 5, "Previous acceleration must contain 5 elements");
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

void IVehicle::checkTrafficLights(std::pair<const TrafficLight*, double> nextTrafficLight) const
{
    if(nextTrafficLight.first == NULL)
    {
        fTrafficLightAccel.first = false;
        return;
    }
    if(fTrafficLightAccel.first)
    {
        if(nextTrafficLight.first->getColor() == TrafficLight::kGreen)
        {
            fTrafficLightAccel.first = false;
            return;
        }
    }
    else
    {
        fTrafficLightAccel = calculateStop(pairPosition<TrafficLight>(nextTrafficLight));
        if(fTrafficLightAccel.first) nextTrafficLight.first->setInRange(this);
    }
}

void IVehicle::checkBusStop(std::pair<const BusStop*, double> nextBusStop) const
{
    if(nextBusStop.first == NULL or getType() != "bus") return;

    if(nextBusStop.first == NULL)
    {
        fTrafficLightAccel.first = false;
        return;
    }
    if(fTrafficLightAccel.first)
    {
        if(std::abs(pairPosition<BusStop>(nextBusStop)) - fPosition < 1)
        {
            nextBusStop.first->setStationed(this);
            return;
        }
    }
    else
    {
        fTrafficLightAccel = calculateStop(pairPosition<BusStop>(nextBusStop));
    }
}

void IVehicle::checkLaneChange(const bool trafficLight, const uint32_t lane, const uint32_t index, Road* const road, const bool left)
{
    // 5. Het voertuig is van type auto of motorfiets.
    if(getType() != "auto" or getType() != "motorfiets") return;

    // 4. Het voertuig rijdt volgens de regels in Use case 3.1, en is dus niet aan het vertragen voor een verkeersteken.
    if(trafficLight) return;

    // 3. Het voertuig heeft 5 seconden op rij een versnelling van 0.
    for(uint32_t i = 0; i < 5; i++) if(fPrevAcceleration[i] > fgkEpsilonThreshold) return;

    // 1. Het voertuig rijdt trager dan de snelheidslimiet van de baan of zone,
    // 2. Het voertuig rijdt trager dan zijn maximaal haalbare snelheid.
    if(left and fVelocity > std::min(road->getSpeedLimit(fPosition), getMaxSpeed())) return;

    // 6. Er is geen voertuig op de nieuwe rijstrook in een straal van de ideale volgafstand (dus zowel voor als achter het voertuig).
    road->changeLaneIfPossible(this, lane, index, left);
}

std::pair<bool, double> IVehicle::calculateStop(double nextPos) const
{
    const double ideal = 0.75 * fVelocity;
    const double dist = nextPos - fPosition;

    if(dist < 2 * ideal) return std::pair<bool, double>(true, -fVelocity*fVelocity/dist);
    else return std::pair<bool, double>(false, 0);
}

void IVehicle::updateStatistics()
{
    fTimer++;
    if(fVelocity > 0) fDriveTimer++;
    fDistance += fVelocity;
    fMaxVelocity = std::max(fVelocity, fMaxVelocity);
}

//--------------------------------------------------------------------------------------------------//

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

double IVehicle::getMinVehicleDist() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getMinVehicleDist");
    return fgkMinVehicleDist;
}

void IVehicle::setMoved(bool moved) const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setStationed");
    fMoved = moved;
}

void IVehicle::setStationed(bool stationed) const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setStationed");
    fStationed = stationed;
}

std::tuple<uint32_t, uint32_t, double, double> IVehicle::getStatistics() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getStatistics");
    return std::tuple<uint32_t, uint32_t, double, double>(fTimer, fDriveTimer, fDistance, fMaxVelocity);
}
