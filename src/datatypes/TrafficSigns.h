//============================================================================
// @name        : TrafficSigns.h
// @author      : Thomas Dooms
// @date        : 4/4/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#ifndef SIMULATION_TRAFFICSIGNS_H
#define SIMULATION_TRAFFICSIGNS_H

#include <stdint.h>
#include <utility>
class IVehicle;


//--------------------------------------------------------------------------------------------------//

class TrafficLight
{
public:
    enum EColor{kRed, kOrange, kGreen};

    /*
     * REQUIRE(kPosition > 0, "kPosition must be greater than 0");
     * ENSURE(properlyInitialized(), "TrafficLight constructor must end in properly initialized state");
     */
    TrafficLight(double kPosition);

    bool properlyInitialized() const;

    /*
     * REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling updateVehicles");
     */
    void update() const;

    /*
     * REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getColor");
     * ENSURE(fTimer <= 90, "timer has exceeded its maximum value");
     */
    EColor getColor() const;

    /*
     * REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getColor");
     * ENSURE(getColor() == color, "new color not set when calling setColor");
     */
    void setColor(EColor color) const;

    /*
     * REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getInRange");
     */
    const IVehicle* getInRange() const;

    /*
     * REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling setInRange");
     * REQUIRE(kVehicle->properlyInitialized(), "kVehicles must be properly initialized");
     * ENSURE(getInRange() == kVehicle, "new in range vehicle not set when calling setInRange");
     */
    void setInRange(const IVehicle* kVehicle) const;

    /*
     * REQUIRE(properlyInitialized(), "TrafficLight was not properly initialized when calling getPosition");
     */
    double getPosition() const;

private:
    double fPosition;

    mutable EColor fColor;
    mutable const IVehicle* fkInRange;

    mutable uint32_t fRedTime;
    mutable uint32_t fGreenTime;
    mutable uint32_t fTimer;

    static const uint32_t fgkMaxDifference;

    const TrafficLight* _initCheck;
};

//--------------------------------------------------------------------------------------------------//

class BusStop
{
public:
    /*
     * REQUIRE(kPosition > 0, "kPosition must be greater than 0");
     * ENSURE(properlyInitialized(), "BusStop constructor must end in properly initialized state");
     */
    BusStop(double kPosition);

    bool properlyInitialized() const;

    /*
     * REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling update");
     * ENSURE(fTimer < fgkStationTime, "Bus stationed for too long");
     */
    void update() const;

    /*
     * REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling getStationed");
     */
    const IVehicle* getStationed() const;

    /*
     * REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling setStationed");
     * REQUIRE(kVehicle->properlyInitialized(), "kVehicles must be properly initialized");
     * ENSURE(getStationed() == kVehicle, "new stationed vehicle not set when calling setStationed");
     */
    void setStationed(const IVehicle* kVehicle) const;

    /*
     * REQUIRE(properlyInitialized(), "BusStop was not properly initialized when calling getPosition");
     */
    double getPosition() const;

private:
    double fPosition;

    mutable const IVehicle* fStationed;
    mutable uint32_t fTimer;

    static const uint32_t fgkStationTime;

    const BusStop* _initCheck;
};

//--------------------------------------------------------------------------------------------------//

class Zone
{
public:
    /*
     * REQUIRE(kPosition   >= 0, "kPosition must be greater than 0"  );
     * REQUIRE(kSpeedLimit >= 0, "kSpeedLimit must be greater than 0");
     * ENSURE(properlyInitialized(), "Zone constructor must end in properly initialized state");
     */
    Zone(double kPosition, double kSpeedLimit);

    bool properlyInitialized() const;

    /*
     * REQUIRE(properlyInitialized(), "Zone was not properly initialized when calling getSpeedlimit");
     */
    double getSpeedlimit() const;

    /*
     * REQUIRE(properlyInitialized(), "Zone was not properly initialized when calling setSpeedLimit");
     * REQUIRE(kSpeedLimit >= 0, "kSpeedLimit must be greater than 0");
     * ENSURE(getSpeedlimit() == kSpeedlimit, "new speedlimit not set when calling setSpeedLimit");
     */
    void setSpeedLimit(double kSpeedlimit) const;

    /*
     * REQUIRE(properlyInitialized(), "Zone was not properly initialized when calling setSpeedLimit");
     */
    double getPosition() const;

private:
    double fPosition;
    mutable double fSpeedlimit;

    const Zone* _initCheck;
};

#endif //SIMULATION_TRAFFICSIGNS_H
