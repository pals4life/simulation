//
// Created by ward on 2/20/19.
//

#include <cmath>
#include "Line2D.h"
#include <algorithm>
#include <cfloat>

img::EasyImage Lines2D::draw(const unsigned int size, const Color &background, const bool zBuffer) const {
    auto xMax = -DBL_MAX;
    auto xMin = DBL_MAX;
    auto yMax = -DBL_MAX;
    auto yMin = DBL_MAX;

    for (const auto &line: lines) {
        if (line.p1.x > xMax) xMax = line.p1.x;
        if (line.p2.x > xMax) xMax = line.p2.x;
        if (line.p1.x < xMin) xMin = line.p1.x;
        if (line.p2.x < xMin) xMin = line.p2.x;
        if (line.p1.y > yMax) yMax = line.p1.y;
        if (line.p2.y > yMax) yMax = line.p2.y;
        if (line.p1.y < yMin) yMin = line.p1.y;
        if (line.p2.y < yMin) yMin = line.p2.y;
    }

    const double xRange = xMax - xMin;
    const double yRange = yMax - yMin;
    const double xImage = size * (xRange / (std::max(xRange, yRange)));
    const double yImage = size * (yRange / (std::max(xRange, yRange)));
    img::EasyImage image(static_cast<unsigned int>(round(xImage)), static_cast<unsigned int>(round(yImage)));
    image.clear(background);
    const double d = 0.95 * xImage / xRange;
    const double DCx = d * ((xMin + xMax) / 2);
    const double DCy = d * ((yMin + yMax) / 2);
    const double dx = (xImage / 2) - DCx;
    const double dy = (yImage / 2) - DCy;

    if (!zBuffer) {
        for (const auto &line: lineptrs) {
            image.draw_line(static_cast<unsigned int>(round((*line).p1.x * d + dx)),
                            static_cast<unsigned int>(round((*line).p1.y * d + dy)),
                            static_cast<unsigned int>(round((*line).p2.x * d + dx)),
                            static_cast<unsigned int>(round((*line).p2.y * d + dy)), (*line).color);
        }
    } else {
        ZBuffer buffer(image.get_width(), image.get_height());
        for (const auto &line: lineptrs) {
            image.draw_zbuf_line(buffer, static_cast<unsigned int>(round((*line).p1.x * d + dx)),
                                 static_cast<unsigned int>(round((*line).p1.y * d + dy)), (*line).p1.z,
                                 static_cast<unsigned int>(round((*line).p2.x * d + dx)),
                                 static_cast<unsigned int>(round((*line).p2.y * d + dy)), (*line).p2.z, (*line).color);
        }
    }
    return image;
}

void Lines2D::addLine(double x1, double y1, double x2, double y2, const Color &color) {
    Point2D a = Point2D(x1, y1);
    Point2D b = Point2D(x2, y2);
    auto first = lines.emplace(std::min(a, b), std::max(a, b), color);
    if (!first.second) {
        (*first.first).color = color;
    } else {
        lineptrs.push_back(first.first);
    }
}

Lines2D::Lines2D(const LParser::LSystem2D &lSystem, const Color &color) {
    Point2D start{0, 0};
    double angle = lSystem.get_starting_angle() * M_PI / 180;
    unsigned int nr = lSystem.get_nr_iterations();
    std::stack<std::pair<Point2D, double>> brackets;
    for (const auto &symbol: lSystem.get_initiator()) {
        drawCharacter(nr, start, angle, lSystem, symbol, brackets, color);
    }
}

void Lines2D::drawCharacter(unsigned int nr, Point2D &start, double &angle, const LParser::LSystem2D &lSystem,
                            const char character, std::stack<std::pair<Point2D, double>> &brackets,
                            const Color &color) {
    switch (character) {
        case '+':
            angle = fmod((angle + (lSystem.get_angle() * M_PI / 180)), (2 * M_PI));
            break;
        case '-':
            angle = fmod((angle - (lSystem.get_angle() * M_PI / 180)), (2 * M_PI));
            break;
        case '(':
            brackets.emplace(start, angle);
            break;
        case ')':
            start = brackets.top().first;
            angle = brackets.top().second;
            brackets.pop();
            break;
        default:
            if (nr == 0) {
                if (lSystem.draw(character)) {
                    addLine(start.x, start.y, start.x + cos(angle), start.y + sin(angle), color);
                }
                start.x += cos(angle);
                start.y += sin(angle);
            } else {
                for (const auto &symbol: lSystem.get_replacement(character)) {
                    drawCharacter(nr - 1, start, angle, lSystem, symbol, brackets, color);
                }
            }
            break;
    }
}

Lines2D::Lines2D(const Figures &figures) {
    for (const auto &figure: figures.getFigures()) {
        for (const auto &face: figure.getFaces()) {
            if (face.point_indexes.size() == 2) {
                const Point2D a = Point2D(figure.getPoints()[face.point_indexes[0]], 1);
                const Point2D b = Point2D(figure.getPoints()[face.point_indexes[1]], 1);
                auto first = lines.emplace(std::min(a, b), std::max(a, b), figure.getAmbient());
                if (!first.second) {
                    (*first.first).color = figure.getAmbient();
                } else {
                    lineptrs.push_back(first.first);
                }
            } else {
                for (unsigned int i = 0; i < face.point_indexes.size(); ++i) {
                    const Point2D a = Point2D(figure.getPoints()[face.point_indexes[i]], 1);
                    const Point2D b = Point2D(
                            figure.getPoints()[face.point_indexes[(i + 1) % face.point_indexes.size()]], 1);
                    auto first = lines.emplace(std::min(a, b), std::max(a, b), figure.getAmbient());
                    if (!first.second) {
                        (*first.first).color = figure.getAmbient();
                    } else {
                        lineptrs.push_back(first.first);
                    }
                }
            }
        }
    }
}


Point2D::Point2D(const Vector3D &point, const double d) {
    x = (d * point.x) / (-point.z);
    y = (d * point.y) / (-point.z);
    z = point.z;
}

Point2D::Point2D(double x, double y) : x(x), y(y) { z = 0; }

bool Point2D::operator<(const Point2D &comp) const {
    if (x < comp.x) return true;
    if (comp.x < x) return false;
    if (y < comp.y) return true;
    if (comp.y < y) return false;
    return z < comp.z;
}

Line2D::Line2D(const Point2D &p1, const Point2D &p2, const Color &color) : p1(p1), p2(p2), color(color) {}

bool Line2D::operator<(const Line2D &comp) const {
    if (p1 < comp.p1) return true;
    if (comp.p1 < p1) return false;
    return p2 < comp.p2;
}
