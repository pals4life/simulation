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

    void update();
    EColor getColor() const;
    void setInRange(const IVehicle* kVehicle) const;
    double getPosition() const;

private:
    EColor fColor;
    double fPosition;
    mutable const IVehicle* fkInRange;

    uint32_t fRedTime;
    uint32_t fGreenTime;
    uint32_t fTimer;

    static const uint32_t fgkMaxDifference;
};

//--------------------------------------------------------------------------------------------------//

class BusStop
{
public:
    BusStop(double kPosition);

    void update();
    void setStationed(IVehicle* vehicle);
    double getPosition() const;

private:
    double fPosition;
    IVehicle* fStationed;

    uint32_t fTimer;
    static const uint32_t stationTime;
};

//--------------------------------------------------------------------------------------------------//

class Zone
{
public:
    Zone(double kPosition, double kSpeedLimit);

    double getPosition() const;
    double getSpeedlimit() const;

private:
    double fPosition;
    double fSpeedlimit;
};

#endif //SIMULATION_TRAFFICSIGNS_H
