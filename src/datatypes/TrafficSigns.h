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

    TrafficLight(double kPosition);
    bool properlyInitialized() const;

    void update() const;
    EColor getColor() const;
    void setInRange(const IVehicle* kVehicle) const;
    double getPosition() const;

private:
    double fPosition;

    mutable EColor fColor;
    mutable const IVehicle* fkInRange;

    mutable uint32_t fRedTime;
    mutable uint32_t fGreenTime;
    mutable uint32_t fTimer;

    static const uint32_t fgkMaxDifference;

    TrafficLight* _initCheck;
};

//--------------------------------------------------------------------------------------------------//

class BusStop
{
public:
    BusStop(double kPosition);
    bool properlyInitialized() const;

    void update() const;
    void setStationed(IVehicle* vehicle) const;
    double getPosition() const;



private:
    double fPosition;

    mutable IVehicle* fStationed;
    mutable uint32_t fTimer;

    static const uint32_t stationTime;

    BusStop* _initCheck;
};

//--------------------------------------------------------------------------------------------------//

class Zone
{
public:
    Zone(double kPosition, double kSpeedLimit);
    bool properlyInitialized() const;

    double getPosition() const;
    double getSpeedlimit() const;

private:
    double fPosition;
    double fSpeedlimit;

    Zone* _initCheck;
};

#endif //SIMULATION_TRAFFICSIGNS_H
