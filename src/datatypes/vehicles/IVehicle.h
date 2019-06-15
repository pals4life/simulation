//============================================================================
// @name        : IVehicle.cpp
// @author      : Thomas Dooms
// @date        : 26/02/2019
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : pure virtual class describing a vehicle
//============================================================================

#ifndef IVEHICLE_H
#define IVEHICLE_H

#include <string>
#include "../TrafficSigns.h"
#include <deque>
#include <tuple>

class Road;

class IVehicle
{
public:
    /**
     * REQUIRE(velocity >= 0, "Velocity must be greater than 0");
     * REQUIRE(position >= 0, "Position must be greater than 0");
     * REQUIRE(!license.empty(), "License plate must be valid");
     *
     * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
     */
    IVehicle(const std::string& license, double pos, double velocity);
    virtual ~IVehicle();

    bool properlyInitialized() const;

    virtual double getVehicleLength() const = 0;
    virtual std::string getType() const = 0;

    virtual double getMaxSpeed() const = 0;
    virtual double getMinSpeed() const = 0;

    virtual double getMaxAcceleration() const = 0;
    virtual double getMinAcceleration() const = 0;

    /*
     * REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");
     * REQUIRE(kRoad->properlyInitialized(), "road was not initialized when calling move");
     * REQUIRE(kRoad->laneExists(kLane), "kLane does not exist when calling move");
     *
     * ENSURE(getVelocity() >= 0, "Velocity cannot be negative");
     * ENSURE((getAcceleration() >= getMinAcceleration()) && (getAcceleration() <= getMinAcceleration()), "Acceleration is too high / low");
     * ENSURE(nextVehicle.first == NULL or pairPosition<IVehicle>(nextVehicle) - getPosition() > getMinVehicleDist(), "distance between vehicles must be greater than minVehicleDist");
     * ENSURE(fPrevAcceleration.size() == 5, "Previous acceleration must contain 5 elements");
     */
    void move(uint32_t kLane, uint32_t kIndex, Road* kRoad);

    //--------------------------------------------------------------------------------------------------//

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
     */
    std::string getLicensePlate()const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
     */
    double getPosition() const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setPosition");
     */
    void setPosition(double position);

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getVelocity");
     */
    double getVelocity() const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getAcceleration");
     */
    double getAcceleration() const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getAcceleration");
     */
    double getMinVehicleDist() const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getMoved");
     */
    bool getMoved() const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMoved");
     */
    void setMoved(bool kMoved) const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getStationed");
     * ENSURE(getMoved() == kMoved, "new moved not set when calling setMoved");
     */
    bool getStationed() const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setStationed");
     * ENSURE(getStationed() == kStationed, "new moved not set when calling setStationed");
     */
    void setStationed(bool kStationed) const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getStationed");
     * ENSURE(getMoved() == kMoved, "new moved not set when calling setMoved");
     */
    bool getMerging() const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setStationed");
     * ENSURE(getStationed() == kStationed, "new moved not set when calling setStationed");
     */
    void setMerging(bool kStationed) const;

    /*
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getStatistics");
     */
    std::tuple<uint32_t, uint32_t, double, double> getStatistics() const;

private:
    /*
     * REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling getFollowingAcceleration");
     * REQUIRE(nextVehicle.second >= 0, "nextVehicle ill-formed when calling getFollowingAcceleration");
     */
    double getFollowingAcceleration(std::pair<const IVehicle*, double> nextVehicle) const;

    /*
     * REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling getMinMaxAcceleration");
     * REQUIRE(speedlimit >= 0, "speedlimit must be greater than 0 when calling checkTrafficLights");
     */
    std::pair<double, double> getMinMaxAcceleration(double speedlimit) const;

    /*
     * REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling checkTrafficLights");
     * REQUIRE(nextTrafficLight.second >= 0, "nextTrafficLight ill-formed when calling checkTrafficLights");
     */
    void checkTrafficLights(std::pair<const TrafficLight*, double> nextTrafficLight) const;

    /*
     * REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling checkBusStop");
     * REQUIRE(nextBusStop.second >= 0, "nextBusStop ill-formed when calling checkBusStop");
     */
    void checkBusStop(std::pair<const BusStop*, double> nextBusStop) const;

    /*
     * REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling checkLaneChange");
     * REQUIRE(road->properlyInitialized(), "road not initialized when calling checkLaneChange");
     * REQUIRE(road->laneExists(lane), "lane does not exist on road when calling checkLaneChange");
     */
    void checkLaneChange(bool trafficLight, uint32_t lane, uint32_t index, Road* road, bool left, double kSpeedlimit);

    /*
     * REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling calculateStop");
     * REQUIRE(nextPos > getPosition(), "cannot stop behind current pos when calling calculate stop");
     */
    std::pair<bool, double> calculateStop(double nextPos) const;

    /*
     * REQUIRE(properlyInitialized(), "Vehicle was not initialized when calling updateStatistics");
     */
    void updateStatistics();

protected:
    IVehicle* _initCheck;

    std::string fLicensePlate;

    mutable bool fMoved;
    mutable bool fStationed;
    mutable bool fMerging;

    double fPosition;
    double fVelocity;
    double fAcceleration;

    std::deque<double> fPrevAcceleration;

    mutable std::tuple<bool, double, const TrafficLight*> fTrafficLightAccel;
    mutable std::tuple<bool, double, const BusStop*> fBusStopAccel;

    uint32_t fTimer;
    uint32_t fDriveTimer;
    double fDistance;
    double fMaxVelocity;

    static const double fgkMinVehicleDist;
    static const double fgkEpsilonThreshold;
};


#endif
