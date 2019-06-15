//
// Created by ward on 2/20/19.
//

#ifndef ENGINE_LINE2D_H
#define ENGINE_LINE2D_H

#include "l_parser/l_parser.h"
#include "Figure.h"
#include <stack>
#include "vector/vector3d.h"
#include "easy_image.h"

struct Point2D {
    double x;
    double y;
    double z;

    Point2D(const Vector3D &point, double d);

    Point2D(double x, double y);

    bool operator<(const Point2D &comp) const;
};

struct Line2D {
    Point2D p1;
    Point2D p2;
    mutable Color color;

    Line2D(const Point2D &p1, const Point2D &p2, const Color &color);

    bool operator<(const Line2D &comp) const;
};

class Lines2D {
    std::vector<std::set<Line2D>::iterator> lineptrs;

    std::set<Line2D> lines;

    void drawCharacter(unsigned int nr, Point2D &start, double &angle, const LParser::LSystem2D &lSystem,
                       char character, std::stack<std::pair<Point2D, double>> &brackets, const Color &color);

public:
    Lines2D(const LParser::LSystem2D &lSystem, const Color &color);

    Lines2D(const Figures &figures);

    img::EasyImage draw(unsigned int size, const Color &background, bool zBuffer) const;

    void addLine(double x1, double y1, double x2, double y2, const Color &color);
};

#endif //ENGINE_LINE2D_H
