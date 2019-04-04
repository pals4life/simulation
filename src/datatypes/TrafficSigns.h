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

class TrafficLight
{
public:
    enum Color{red, orange, green};

    void update() const;
    Color getColor() const;
    double getPosition() const{ return fPosition; }

    friend bool operator<(const double a, const TrafficLight& b);

private:
    Color fColor;
    double fPosition;
};


struct BusStop
{
    double fPosition;
    IVehicle* stationed;

    void update();
    double getPosition() const{ return fPosition; }

    friend bool operator<(const double a, const BusStop& b);
};

struct Zone
{
    double fPosition;
    double fSpeedlimit;

    friend bool operator<(const double a, const Zone& b);
};

#endif //SIMULATION_TRAFFICSIGNS_H
