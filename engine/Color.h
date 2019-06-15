//============================================================================
// @name        : Color.h
// @author      : Ward Gauderis
// @date        : 4/28/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#ifndef ENGINE_CMAKE_COLOR_H
#define ENGINE_CMAKE_COLOR_H


struct Color {
    double red;
    double green;
    double blue;

    Color() {
        red = 0;
        green = 0;
        blue = 0;
    }

    Color(const std::vector<double> &color) {
        red = color[0];
        green = color[1];
        blue = color[2];
    }

    Color(const double r, const double g, const double b) {
        red = r;
        green = g;
        blue = b;
    }

    bool operator<(const Color &comp) const {
        if (red < comp.red) return true;
        if (comp.red < red) return false;
        if (green < comp.green) return true;
        if (comp.green < green) return false;
        return blue < comp.blue;
    }

    Color &operator+=(const Color &color) {
        red += color.red;
        green += color.green;
        blue += color.blue;
        return *this;
    }

    Color operator+(const Color &color) const {
        Color c = *this;
        c += color;
        return c;
    }

    Color &operator*=(const Color &color) {
        red *= color.red;
        green *= color.green;
        blue *= color.blue;
        return *this;
    }

    Color operator*(const Color &color) const {
        Color c = *this;
        c *= color;
        return c;
    }

    Color &operator*=(const double value) {
        red *= value;
        green *= value;
        blue *= value;
        return *this;
    }

    Color operator*(const double value) const {
        Color c = *this;
        c *= value;
        return c;
    }
};

#endif //ENGINE_CMAKE_COLOR_H
