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
    fMerging = false;

    fLicensePlate = license;

    fPosition = position;
    fVelocity = velocity;
    fAcceleration = 0;

    fPrevAcceleration = std::deque<double>(5, 1);

    fTrafficLightAccel = std::tuple<bool, double, const TrafficLight*>(false, 0, NULL);
    fBusStopAccel = std::tuple<bool, double, const BusStop*>(false, 0, NULL);

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

void IVehicle::move(const uint32_t kLane, const uint32_t kIndex, Road* const kRoad)
{
    REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");
    REQUIRE(kRoad->properlyInitialized(), "road was not initialized when calling move");
    REQUIRE(kRoad->laneExists(kLane), "kLane does not exist when calling move");

    if(fMoved) return;      // moved means the vehicle already has been updated
    fMoved = true;

    updateStatistics();
    if(fStationed) return;  // stationed means the vehicle must not update

    checkTrafficLights(kRoad->getTrafficLight(fPosition));                                              // calculate the slowdown if needed
    checkBusStop(kRoad->getBusStop(fPosition));                                                         // calculate the slowdown if needed
    const double kSpeedlimit = kRoad->getSpeedLimit(fPosition);                                         // calculate the speed limit

    const std::pair<const IVehicle*, double> kNextVehicle = kRoad->getNextVehicle(kLane, kIndex);       // get the next vehicle
    if(kNextVehicle.first != NULL and not kNextVehicle.first->getMoved()) kRoad->updateNextVehicles();  // if the next road hasnt been updated yet, update it.
    double acceleration = getFollowingAcceleration(kNextVehicle);                                       // calculate the following speed

    if(std::get<0>(fTrafficLightAccel)) acceleration = std::min(std::get<1>(fTrafficLightAccel), acceleration);    // we need to take the minimum of these values
    if(std::get<0>(fBusStopAccel)     ) acceleration = std::min(std::get<1>(fBusStopAccel)     , acceleration);    // to ensure we slow down enough

    const std::pair<double, double> kMinMax = getMinMaxAcceleration(kSpeedlimit);                       // calculate the min and max acceleration possible

    fAcceleration = clamp(acceleration, kMinMax.first, kMinMax.second);                                 // clamp the values
    fVelocity += fAcceleration;                                                                         // Calculate new velocity
    fPosition += fVelocity;                                                                             // Calculate new positions

    fPrevAcceleration.push_front(fAcceleration);
    fPrevAcceleration.pop_back();

    if(kRoad->laneExists(kLane+1) and !fMerging) checkLaneChange(std::get<0>(fTrafficLightAccel), kLane, kIndex, kRoad, true , kSpeedlimit);// overtake if possible
    if(kRoad->laneExists(kLane-1) and !fMerging) checkLaneChange(std::get<0>(fTrafficLightAccel), kLane, kIndex, kRoad, false, kSpeedlimit);// go back if possible

    ENSURE(getVelocity() >= 0, "Velocity cannot be negative");
    ENSURE((getAcceleration() >= getMinAcceleration()) && (getAcceleration() <= getMaxAcceleration()), "Acceleration is too high / low");
    ENSURE(kNextVehicle.first == NULL or pairPosition<IVehicle>(kNextVehicle) - getPosition() > getMinVehicleDist(), "distance between vehicles must be greater than minVehicleDist");
    ENSURE(fPrevAcceleration.size() == 5, "Previous acceleration must contain 5 elements");
}

double IVehicle::getFollowingAcceleration(std::pair<const IVehicle*, double> nextVehicle) const
{
    REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling getFollowingAcceleration");
    REQUIRE(nextVehicle.second >= 0, "nextVehicle ill-formed when calling getFollowingAcceleration");

    if(nextVehicle.first == NULL) return getMaxAcceleration();                                                  // if there is not car in front, acceleration = max

    double ideal;
    if(fVelocity - nextVehicle.first->getVelocity() > -getMinAcceleration())
    {
        ideal  = 1.5 * fVelocity + nextVehicle.first->getVehicleLength() + 2;                               // ideal following distance = 3/4 speed + 2 meters extra
    }
    else
    {
        ideal  = 0.75 * fVelocity + nextVehicle.first->getVehicleLength() + 2;                               // ideal following distance = 3/4 speed + 2 meters extra
    }
    double actual = pairPosition<IVehicle>(nextVehicle) - nextVehicle.first->getVelocity() - nextVehicle.first->getVehicleLength() - fPosition;    // distance between 2 vehicles


    return 0.5 * (actual - ideal);                                                                              // take the average
}

std::pair<double, double> IVehicle::getMinMaxAcceleration(double speedlimit) const
{
    REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling getMinMaxAcceleration");
    REQUIRE(speedlimit >= 0, "speedlimit must be greater than 0 when calling getMinMaxAcceleration");

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
    REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling checkTrafficLights");
    REQUIRE(nextTrafficLight.second >= 0, "nextTrafficLight ill-formed when calling checkTrafficLights");

    if(std::get<0>(fTrafficLightAccel))
    {
        if(nextTrafficLight.first == NULL or nextTrafficLight.first->getColor() == TrafficLight::kGreen)
        {
            std::get<0>(fTrafficLightAccel) = false;
            return;
        }
    }
    else if(nextTrafficLight.first != NULL and nextTrafficLight.first->getColor() != TrafficLight::kGreen)
    {
        std::pair<bool, double> temp = calculateStop(pairPosition<TrafficLight>(nextTrafficLight));
        std::get<0>(fTrafficLightAccel) = temp.first;
        std::get<1>(fTrafficLightAccel) = temp.second;
        std::get<2>(fTrafficLightAccel) = (temp.first) ? nextTrafficLight.first : NULL;

        if(std::get<0>(fTrafficLightAccel))
            nextTrafficLight.first->setInRange(this);
    }
}

void IVehicle::checkBusStop(std::pair<const BusStop*, double> nextBusStop) const
{
    REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling checkBusStop");
    REQUIRE(nextBusStop.second >= 0, "nextBusStop ill-formed when calling checkBusStop");

    if(getType() != "bus") return;

    if(std::get<0>(fBusStopAccel))
    {
        if(fVelocity < fgkEpsilonThreshold)
        {
            std::get<2>(fBusStopAccel)->setStationed(this);
            fBusStopAccel = std::tuple<bool, double, const BusStop*>(false, 0, NULL);
            return;
        }
    }
    else if(nextBusStop.first != NULL)
    {
        std::pair<bool, double> temp = calculateStop(pairPosition<BusStop>(nextBusStop));
        std::get<0>(fBusStopAccel) = temp.first;
        std::get<1>(fBusStopAccel) = temp.second;
        std::get<2>(fBusStopAccel) = (temp.first) ? nextBusStop.first : NULL;
    }
}

void IVehicle::checkLaneChange(const bool trafficLight, const uint32_t lane, const uint32_t index, Road* const road, const bool left, const double kSpeedlimit)
{
    REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling checkLaneChange");
    REQUIRE(road->laneExists(lane), "lane does not exist on road when calling checkLaneChange");

    // 5. Het voertuig is van type auto of motorfiets.
    if(getType() != "auto" and getType() != "motorfiets") return;

    // 4. Het voertuig rijdt volgens de regels in Use case 3.1, en is dus niet aan het vertragen voor een verkeersteken.
    if(trafficLight) return;

    // 3. Het voertuig heeft 5 seconden op rij een versnelling van 0.
    for(uint32_t i = 0; i < 5; i++) if(fPrevAcceleration[i] > fgkEpsilonThreshold) return;

    // 1. Het voertuig rijdt trager dan de snelheidslimiet van de baan of zone,
    // 2. Het voertuig rijdt trager dan zijn maximaal haalbare snelheid.
    if(left and fVelocity >= std::min(kSpeedlimit, getMaxSpeed()) - fgkEpsilonThreshold) return;

    // 6. Er is geen voertuig op de nieuwe rijstrook in een straal van de ideale volgafstand (dus zowel voor als achter het voertuig).
    fMerging = road->changeLaneIfPossible(this, lane, index, left);
}

std::pair<bool, double> IVehicle::calculateStop(double nextPos) const
{
    REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling calculateStop");
    REQUIRE(nextPos > getPosition(), "cannot stop behind current pos when calling calculateStop");

    const double kDist = nextPos - fPosition;
    const double kAccel = -fVelocity*fVelocity/(kDist + 2*fVelocity);

    if(kDist < 10) return std::pair<bool, double>(false, 0);

    double futurePos = fPosition + fVelocity + getMaxAcceleration();
    double futureVel = fVelocity;
    while(futureVel > -getMinAcceleration())
    {
        futureVel += getMinAcceleration();
        futurePos += futureVel;
    }
    futurePos += futureVel;

    if(futurePos-fVelocity > nextPos)
    {
        return std::pair<bool, double>(true, kAccel);
    }

    else return std::pair<bool, double>(false, 0);


//    const double ideal = 0.75 * fVelocity;
//    const double dist = nextPos - fPosition;
//
//    // we will stop one tick before we reach the stop zone, because busses are barely able to stop in 2 * ideal dist.
//    std::pair<bool, double> result;

//    if(dist-fVelocity < 2 * ideal) result = std::pair<bool, double>(true, -fVelocity*fVelocity/(dist+3*fVelocity + fgkEpsilonThreshold));
//    else result = std::pair<bool, double>(false, 0);
//
//    if(result.second < getMinAcceleration()) result.second = getMinAcceleration();
//    ENSURE(result.second >= getMinAcceleration(), "Vehicle cannot stop faster than minimum acceleration");
//
//    return result;
}

void IVehicle::updateStatistics()
{
    REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling updateStatistics");
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

bool IVehicle::getMoved() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getMoved");
    return fMoved;
}

void IVehicle::setMoved(const bool kMoved) const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMoved");
    fMoved = kMoved;
    ENSURE(getMoved() == kMoved, "new moved not set when calling setMoved");
}

bool IVehicle::getStationed() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getStationed");
    return fStationed;
}

void IVehicle::setStationed(bool kStationed) const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setStationed");
    fStationed = kStationed;
    ENSURE(getStationed() == kStationed, "new moved not set when calling setStationed");
}

bool IVehicle::getMerging() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getMerging");
    return fMerging;
}

void IVehicle::setMerging(bool kMerging) const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMerging");
    fMerging = kMerging;
    ENSURE(getMerging() == kMerging, "new moved not set when calling setMerging");
}

std::tuple<uint32_t, uint32_t, double, double> IVehicle::getStatistics() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getStatistics");
    return std::tuple<uint32_t, uint32_t, double, double>(fTimer, fDriveTimer, fDistance, fMaxVelocity);
}
