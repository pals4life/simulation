//============================================================================
// @name        : Light.h
// @author      : Ward Gauderis
// @date        : 4/28/19
// @version     : 1.0
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#ifndef ENGINE_CMAKE_LIGHT_H
#define ENGINE_CMAKE_LIGHT_H

#include <cfloat>
#include <algorithm>
#include "Color.h"

struct PointLight {
    Color ambient;
    Color diffuse;
    Color specular;
    Vector3D location;
    ZBuffer shadowMask;
    Matrix eye;
    double d, dx, dy;

    PointLight(const Color &ambient, const Color &diffuse, const Color &specular, const Vector3D &location) : ambient(
            ambient), diffuse(diffuse), specular(specular), location(location) {};

    PointLight &operator*=(const Matrix &matrix) {
        location *= matrix;
        return *this;
    };
};

struct InfLight {
    Color ambient;
    Color diffuse;
    Color specular;
    Vector3D direction;

    InfLight(const Color &ambient, const Color &diffuse, const Color &specular, const Vector3D &direction) : ambient(
            ambient), diffuse(diffuse), specular(specular), direction(Vector3D::normalise(direction)) {};

    InfLight &operator*=(const Matrix &matrix) {
        direction *= matrix;
        return *this;
    };
};

struct PointLights : public std::vector<PointLight> {
    PointLights &operator*=(const Matrix &matrix) {
        for (auto &light: *this) {
            light *= matrix;
        }
        return *this;
    };
};

struct InfLights : public std::vector<InfLight> {
    InfLights &operator*=(const Matrix &matrix) {
        for (auto &light: *this) {
            light *= matrix;
        }
        return *this;
    };
};

#endif //ENGINE_CMAKE_LIGHT_H
