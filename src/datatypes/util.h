//============================================================================
// @name        : util.h
// @author      : Thomas Dooms
// @date        : 4/21/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#ifndef SIMULATION_UTIL_H
#define SIMULATION_UTIL_H


#include <utility>
#include <algorithm>

template<typename T>
double pairPosition(std::pair<const T*, double> next)
{
    return next.first->getPosition() + next.second;
}

template<typename T>
bool comparePosition(double lhs, const T* rhs)
{
    return lhs < rhs->getPosition();
}

template<typename T>
bool comparePositions(const T* lhs, const T* rhs)
{
    return lhs->getPosition() < rhs->getPosition();
}

struct Comparator : public std::unary_function<Road*, bool>
{
    explicit Comparator(const std::string& base) : base(base) {}
    bool operator()(Road* road) { return base == road->getName(); }

    const std::string& base;
};

template<typename T>
typename std::vector<const T*>::iterator insert_sorted(std::vector<const T*>& vec, const T* item)
{
    return vec.insert(std::upper_bound(vec.begin(), vec.end(), item, comparePositions<T>), item);
}


#endif //SIMULATION_UTIL_H
