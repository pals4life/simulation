//============================================================================
// @name        : Figure.h
// @author      : Ward Gauderis
// @date        : 3/03/2019
//============================================================================

#ifndef ENGINE_CMAKE_FIGURE_H
#define ENGINE_CMAKE_FIGURE_H

#include <forward_list>
#include "l_parser/l_parser.h"
#include "vector/vector3d.h"
#include "ini_configuration.h"
#include <stack>
#include "easy_image.h"

void toPolar(const Vector3D &point, double &theta, double &phi, double &r);

Matrix scaleFigure(double scale);

Matrix rotateX(double angle);

Matrix rotateY(double angle);

Matrix rotateZ(double angle);

Matrix translate(const Vector3D &vector);

Matrix eyePoint(const Vector3D &eyepoint);

struct Face {
    std::vector<int> point_indexes;

    Face(const std::initializer_list<int> &point_indexes);

    Face(std::vector<int> point_indexes);

    void triangulate(std::vector<Face> &faces);

    Face() = default;
};

class Figure {
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    Color ambient;
    Color diffuse;
    Color specular;
    double reflectionCoefficient{};
    std::string texture;
    Vector3D p;
    Vector3D a;
    Vector3D b;

    static void sort(std::vector<Face *> &faces, int index);

public:
    void setTexture(const Figure &figure);

    void setColor(const Figure &figure);

    Figure(const std::string &name, const ini::Configuration &configuration);

    const std::string &getTexture() const;

    const Vector3D &getP() const;

    const Vector3D &getA() const;

    const Vector3D &getB() const;

    bool isTextured() const;

    void setTexture(const std::string &tex, const Vector3D &pos, const Vector3D &x, const Vector3D &y);

    void setColor(const Color &a, const Color &d, const Color &s, double r);

    const Color &getAmbient() const;

    const Color &getDiffuse() const;

    const Color &getSpecular() const;

    double getReflectionCoefficient() const;

    Figure &operator*=(const Matrix &matrix);

    Figure operator*(const Matrix &matrix) const;

    const std::vector<Vector3D> &getPoints() const;

    const std::vector<Face> &getFaces() const;

    void addPoint(const Vector3D &vector);

    void addFace(const Face &face);

    void normalize();

    static Figure cube();

    static Figure tetrahedron();

    static Figure octahedron();

    static Figure icosahedron();

    static Figure buckyball();

    static Figure dodecahedron();

    static Figure cylinder(int n, double height, bool faces);

    static Figure cone(int n, double height);

    static Figure sphere(int n);

    static Figure Torus(double R, double r, int n, int m);

    Figure(const LParser::LSystem3D &lSystem);

    void triangulate();

    Figure();

    void drawCharacter(unsigned int nr, Vector3D &start, Vector3D &H, Vector3D &L, Vector3D &U,
                       const LParser::LSystem3D &lSystem, char character,
                       std::stack<std::tuple<Vector3D, Vector3D, Vector3D, Vector3D, int>> &brackets,
                       double angle, int &prevPoint, bool &changed);
};

class Figures {
    std::forward_list<Figure> figures;

    static void mengerRec(Figures &figs, int iter, double scale, const Vector3D &corner);

    void addCilinder(const Vector3D &begin, const Vector3D &end, int n, double radius, const Figure &fig);

public:
    void setTexture(const std::string &tex, const Vector3D &pos, const Vector3D &x, const Vector3D &y);

    void setColor(const Color &a, const Color &d, const Color &s, double r);

    Figures &operator*=(const Matrix &matrix);

    Figures &operator+=(Figures &&figs);

    Figures operator*(const Matrix &matrix) const;

    const std::forward_list<Figure> &getFigures() const;

    void triangulate();

    void addFigure(Figure &&figure);

    std::tuple<double, double, double, double, double> calculateValues(unsigned int size) const;

    img::EasyImage
    draw(unsigned int size, const Color &background, const PointLights &point, const InfLights &inf, const Matrix &eye,
         bool shadows) const;

    static Figures fractal(Figure &figure, int iter, double scale);

    Figures(const Figure &fig, double r, int n, int m);

    Figures();

    static void fractalRec(Figures &figs, const Figure &fig, int iter, double scale);

    static Figures mengerSponge(int iter);

    void generateShadowMasks(PointLights &points, unsigned int size);
};

#endif //ENGINE_CMAKE_FIGURE_H
