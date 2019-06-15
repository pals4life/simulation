//============================================================================
// @name        : ZBuffer.h
// @author      : Ward Gauderis
// @date        : 19/03/2019
// @version     : 
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#ifndef ENGINE_CMAKE_ZBUFFER_H
#define ENGINE_CMAKE_ZBUFFER_H

#include <vector>

class ZBuffer : public std::vector<std::vector<double>> {
public:
    ZBuffer() = default;

    ZBuffer(unsigned int width, unsigned int height);
};


#endif //ENGINE_CMAKE_ZBUFFER_H
