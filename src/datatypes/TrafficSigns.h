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


template<typename T>
double pairPosition(std::pair<const T*, double> next)
{
    return next.first->getPosition() + next.second;
}

//--------------------------------------------------------------------------------------------------//

class TrafficLight
{
public:
    enum Color{red, orange, green};

    TrafficLight(double kPosition);

    void update();
    Color getColor() const;
    void setInRange(const IVehicle* vehicle);
    double getPosition() const;

    friend bool operator<(double lhs, const TrafficLight& rhs);

private:
    Color fColor;
    double fPosition;
    const IVehicle* fInRange;

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

    friend bool operator<(double lhs, const BusStop& rhs);

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

    friend bool operator<(double lhs, const Zone& rhs);

private:
    double fPosition;
    double fSpeedlimit;
};

#endif //SIMULATION_TRAFFICSIGNS_H
