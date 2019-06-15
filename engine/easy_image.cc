/*
 * easy_image.cc
 * Copyright (C) 2011  Daniel van den Akker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "easy_image.h"
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <cfloat>

#define le32toh(x) (x)

namespace {
    //structs borrowed from wikipedia's article on the BMP file format
    struct bmpfile_magic {
        uint8_t magic[2];
    };

    struct bmpfile_header {
        uint32_t file_size;
        uint16_t reserved_1;
        uint16_t reserved_2;
        uint32_t bmp_offset;
    };
    struct bmp_header {
        uint32_t header_size;
        int32_t width;
        int32_t height;
        uint16_t nplanes;
        uint16_t bits_per_pixel;
        uint32_t compress_type;
        uint32_t pixel_size;
        int32_t hres;
        int32_t vres;
        uint32_t ncolors;
        uint32_t nimpcolors;
    };

    //copy-pasted from lparser.cc to allow these classes to be used independently from each other
    class enable_exceptions {
    private:
        std::ios &ios;
        std::ios::iostate state;
    public:
        enable_exceptions(std::ios &an_ios, std::ios::iostate exceptions) :
                ios(an_ios) {
            state = ios.exceptions();
            ios.exceptions(exceptions);
        }

        ~enable_exceptions() {
            ios.exceptions(state);
        }
    };

    //helper function to convert a number (char, int, ...) to little endian
    //regardless of the endiannes of the system
    //more efficient machine-dependent functions exist, but this one is more portable
    template<typename T>
    T to_little_endian(T value) {
        //yes, unions must be used with caution, but this is a case in which a union is needed
        union {
            T t;
            uint8_t bytes[sizeof(T)];
        } temp_storage;

        for (uint8_t i = 0; i < sizeof(T); i++) {
            temp_storage.bytes[i] = value & 0xFF;
            value >>= 8;
        }
        return temp_storage.t;
    }

    template<typename T>
    T from_little_endian(T value) {
        //yes, unions must be used with caution, but this is a case in which a union is needed
        union {
            T t;
            uint8_t bytes[sizeof(T)];
        } temp_storage;
        temp_storage.t = value;
        T retVal = 0;

        for (uint8_t i = 0; i < sizeof(T); i++) {
            retVal = (retVal << 8) | temp_storage.bytes[sizeof(T) - i - 1];
        }
        return retVal;
    }

}

img::Color::Color() :
        blue(0), green(0), red(0) {
}

img::Color::Color(uint8_t r, uint8_t g, uint8_t b) :
        blue(b), green(g), red(r) {
}

img::Color::~Color() {
}

img::UnsupportedFileTypeException::UnsupportedFileTypeException(std::string const &msg) :
        message(msg) {
}

img::UnsupportedFileTypeException::UnsupportedFileTypeException(const UnsupportedFileTypeException &original)
        : std::exception(original), message(original.message) {
}

img::UnsupportedFileTypeException::~UnsupportedFileTypeException() throw() {
}

img::UnsupportedFileTypeException &
img::UnsupportedFileTypeException::operator=(UnsupportedFileTypeException const &original) {
    this->message = original.message;
    return *this;
}

const char *img::UnsupportedFileTypeException::what() const throw() {
    return message.c_str();
}

img::EasyImage::EasyImage() :
        width(0), height(0), bitmap() {
}

img::EasyImage::EasyImage(unsigned int _width, unsigned int _height, Color color) :
        width(_width), height(_height), bitmap(width * height, color) {
}

img::EasyImage::EasyImage(EasyImage const &img) :
        width(img.width), height(img.height), bitmap(img.bitmap) {
}

img::EasyImage::~EasyImage() {
    bitmap.clear();
}

img::EasyImage &img::EasyImage::operator=(img::EasyImage const &img) {
    width = img.width;
    height = img.height;
    bitmap.assign(img.bitmap.begin(), img.bitmap.end());
    return (*this);
}

unsigned int img::EasyImage::get_width() const {
    return width;
}

unsigned int img::EasyImage::get_height() const {
    return height;
}

void img::EasyImage::clear(Color color) {
    for (std::vector<Color>::iterator i = bitmap.begin(); i != bitmap.end(); i++) {
        *i = color;
    }
}

img::Color &img::EasyImage::operator()(unsigned int x, unsigned int y) {
    assert(x < this->width);
    assert(y < this->height);
    return bitmap.at(x * height + y);
}

img::Color const &img::EasyImage::operator()(unsigned int x, unsigned int y) const {
    assert(x < this->width);
    assert(y < this->height);
    return bitmap.at(x * height + y);
}

void img::EasyImage::draw_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Color color) {
    assert(x0 < this->width && y0 < this->height);
    assert(x1 < this->width && y1 < this->height);
    if (x0 == x1) {
        //special case for x0 == x1
        for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++) {
            (*this)(x0, i) = color;
        }
    } else if (y0 == y1) {
        //special case for y0 == y1
        for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++) {
            (*this)(i, y0) = color;
        }
    } else {
        if (x0 > x1) {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0) {
            for (unsigned int i = 0; i <= (x1 - x0); i++) {
                (*this)(x0 + i, (unsigned int) round(y0 + m * i)) = color;
            }
        } else if (m > 1.0) {
            for (unsigned int i = 0; i <= (y1 - y0); i++) {
                (*this)((unsigned int) round(x0 + (i / m)), y0 + i) = color;
            }
        } else if (m < -1.0) {
            for (unsigned int i = 0; i <= (y0 - y1); i++) {
                (*this)((unsigned int) round(x0 - (i / m)), y0 - i) = color;
            }
        }
    }
}

std::ostream &img::operator<<(std::ostream &out, EasyImage const &image) {

    //temporaryily enable exceptions on output stream
    enable_exceptions(out, std::ios::badbit | std::ios::failbit);
    //declare some struct-vars we're going to need:
    bmpfile_magic magic;
    bmpfile_header file_header;
    bmp_header header;
    uint8_t padding[] =
            {0, 0, 0, 0};
    //calculate the total size of the pixel data
    unsigned int line_width = image.get_width() * 3; //3 bytes per pixel
    unsigned int line_padding = 0;
    if (line_width % 4 != 0) {
        line_padding = 4 - (line_width % 4);
    }
    //lines must be aligned to a multiple of 4 bytes
    line_width += line_padding;
    unsigned int pixel_size = image.get_height() * line_width;

    //start filling the headers
    magic.magic[0] = 'B';
    magic.magic[1] = 'M';

    file_header.file_size = to_little_endian(pixel_size + sizeof(file_header) + sizeof(header) + sizeof(magic));
    file_header.bmp_offset = to_little_endian(sizeof(file_header) + sizeof(header) + sizeof(magic));
    file_header.reserved_1 = 0;
    file_header.reserved_2 = 0;
    header.header_size = to_little_endian(sizeof(header));
    header.width = to_little_endian(image.get_width());
    header.height = to_little_endian(image.get_height());
    header.nplanes = to_little_endian(1);
    header.bits_per_pixel = to_little_endian(24);//3bytes or 24 bits per pixel
    header.compress_type = 0; //no compression
    header.pixel_size = pixel_size;
    header.hres = to_little_endian(11811); //11811 pixels/meter or 300dpi
    header.vres = to_little_endian(11811); //11811 pixels/meter or 300dpi
    header.ncolors = 0; //no color palette
    header.nimpcolors = 0;//no important colors

    //okay that should be all the header stuff: let's write it to the stream
    out.write((char *) &magic, sizeof(magic));
    out.write((char *) &file_header, sizeof(file_header));
    out.write((char *) &header, sizeof(header));

    //okay let's write the pixels themselves:
    //they are arranged left->right, bottom->top, b,g,r
    for (unsigned int i = 0; i < image.get_height(); i++) {
        //loop over all lines
        for (unsigned int j = 0; j < image.get_width(); j++) {
            //loop over all pixels in a line
            //we cast &color to char*. since the color fields are ordered blue,green,red they should be written automatically
            //in the right order
            out.write((char *) &image(j, i), 3 * sizeof(uint8_t));
        }
        if (line_padding > 0)
            out.write((char *) padding, line_padding);
    }
    //okay we should be done
    return out;
}

std::istream &img::operator>>(std::istream &in, EasyImage &image) {
    enable_exceptions(in, std::ios::badbit | std::ios::failbit);
    //declare some struct-vars we're going to need
    bmpfile_magic magic;
    bmpfile_header file_header;
    bmp_header header;
    //a temp buffer for reading the padding at the end of each line
    uint8_t padding[] =
            {0, 0, 0, 0};

    //read the headers && do some sanity checks
    in.read((char *) &magic, sizeof(magic));
    if (magic.magic[0] != 'B' || magic.magic[1] != 'M')
        throw UnsupportedFileTypeException("Could not parse BMP File: invalid magic header");
    in.read((char *) &file_header, sizeof(file_header));
    in.read((char *) &header, sizeof(header));
    if (le32toh(header.pixel_size) + le32toh(file_header.bmp_offset) != le32toh(file_header.file_size))
        throw UnsupportedFileTypeException("Could not parse BMP File: file size mismatch");
    if (le32toh(header.header_size) != sizeof(header))
        throw UnsupportedFileTypeException("Could not parse BMP File: Unsupported BITMAPV5HEADER size");
    if (le32toh(header.compress_type) != 0)
        throw UnsupportedFileTypeException("Could not parse BMP File: Only uncompressed BMP files can be parsed");
    if (le32toh(header.nplanes) != 1)
        throw UnsupportedFileTypeException("Could not parse BMP File: Only one plane should exist in the BMP file");
    if (le32toh(header.bits_per_pixel) != 24)
        throw UnsupportedFileTypeException("Could not parse BMP File: Only 24bit/pixel BMP's are supported");
    //if height<0 -> read top to bottom instead of bottom to top
    bool invertedLines = from_little_endian(header.height) < 0;
    image.height = std::abs(from_little_endian(header.height));
    image.width = std::abs(from_little_endian(header.width));
    unsigned int line_padding = from_little_endian(header.pixel_size) / image.height - (3 * image.width);
    //re-initialize the image bitmap
    image.bitmap.clear();
    image.bitmap.assign(image.height * image.width, Color());
    //okay let's read the pixels themselves:
    //they are arranged left->right., bottom->top if height>0, top->bottom if height<0, b,g,r
    for (unsigned int i = 0; i < image.get_height(); i++) {
        //loop over all lines
        for (unsigned int j = 0; j < image.get_width(); j++) {
            //loop over all pixels in a line
            //we cast &color to char*. since the color fields are ordered blue,green,red, the data read should be written in the right variables
            if (invertedLines) {
                //store top-to-bottom
                in.read((char *) &image(j, image.height - 1 - i), 3 * sizeof(uint8_t));
            } else {
                //store bottom-to-top
                in.read((char *) &image(j, i), 3 * sizeof(uint8_t));
            }
        }
        if (line_padding > 0) {
            in.read((char *) padding, line_padding);
        }
    }
    //okay we're done
    return in;
}

void
img::EasyImage::draw_zbuf_line(ZBuffer &zBuffer, unsigned int x0, unsigned int y0, double z0, unsigned int x1,
                               unsigned int y1, double z1, const img::Color color) {
    assert(x0 < this->width && y0 < this->height);
    assert(x1 < this->width && y1 < this->height);
    double z = z0;
    int pixels = std::max(abs(int(x0 - x1)), abs(int(y0 - y1)));
    double step = (z1 - z0) / (pixels);
    if (x0 == x1) {
        //special case for x0 == x1
        if (y0 > y1) {
            std::swap(y0, y1);
            z = z1;
            step = -step;
        }
        for (unsigned int i = y0; i <= y1; i++) {
            if (1 / z < zBuffer[x0][i]) {
                (*this)(x0, i) = color;
                zBuffer[x0][i] = 1 / z;
            }
            z += step;
        }
    } else if (y0 == y1) {
        //special case for y0 == y1
        if (x0 > x1) {
            std::swap(x0, x1);
            z = z1;
            step = -step;
        }
        for (unsigned int i = x0; i <= x1; i++) {
            if (1 / z <= zBuffer[i][y0]) {
                (*this)(i, y0) = color;
                zBuffer[i][y0] = 1 / z;
            }
            z += step;
        }
    } else {
        if (x0 > x1) {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
            z = z1;
            step = -step;
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0) {
            for (unsigned int i = 0; i <= (x1 - x0); i++) {
                if (1 / z <= zBuffer[x0 + i][(unsigned int) round(y0 + m * i)]) {
                    (*this)(x0 + i, (unsigned int) round(y0 + m * i)) = color;
                    zBuffer[x0 + i][(unsigned int) round(y0 + m * i)] = 1 / z;
                }
                z += step;
            }
        } else if (m > 1.0) {
            for (unsigned int i = 0; i <= (y1 - y0); i++) {
                if (1 / z <= zBuffer[(unsigned int) round(x0 + (i / m))][y0 + i]) {
                    (*this)((unsigned int) round(x0 + (i / m)), y0 + i) = color;
                    zBuffer[(unsigned int) round(x0 + (i / m))][y0 + i] = 1 / z;
                }
                z += step;
            }
        } else if (m < -1.0) {
            for (unsigned int i = 0; i <= (y0 - y1); i++) {
                if (1 / z <= zBuffer[(unsigned int) round(x0 - (i / m))][y0 - i]) {
                    (*this)((unsigned int) round(x0 - (i / m)), y0 - i) = color;
                    zBuffer[(unsigned int) round(x0 - (i / m))][y0 - i] = 1 / z;
                }
                z += step;
            }
        }
    }
}

void
img::EasyImage::draw_triangle(ZBuffer &zBuffer, const Vector3D &a, const Vector3D &b, const Vector3D &c, double d,
                              double dx, double dy,
                              const ::Color &ambient, const ::Color &diffuse, const ::Color &specular,
                              const double coefficient,
                              const PointLights &points, const InfLights &infs, const ::Color &totalAmbient,
                              const Matrix &eye,
                              const bool shadows) {
    if ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) < 0)  //backface culling
        return;

    struct point {
        double x;
        double y;
    } const A{(d * a.x / -a.z) + dx, (d * a.y / -a.z) + dy}, B{(d * b.x / -b.z) + dx, (d * b.y / -b.z) + dy}, C{
            (d * c.x / -c.z) + dx, (d * c.y / -c.z) + dy};

    assert(A.x < this->width && A.y < this->height);
    assert(B.x < this->width && B.y < this->height);
    assert(C.x < this->width && C.y < this->height);

    const unsigned int ymin = static_cast<int>(round(std::min(std::min(A.y, B.y), C.y) + 0.5));
    const unsigned int ymax = static_cast<int>(round(std::max(std::max(A.y, B.y), C.y) - 0.5));
    const double xg = (A.x + B.x + C.x) / 3;
    const double yg = (A.y + B.y + C.y) / 3;
    const double zg = 1.0 / (3 * a.z) + 1.0 / (3 * b.z) + 1.0 / (3 * c.z);
    const Vector3D u = b - a;
    const Vector3D v = c - a;
    const Vector3D w = Vector3D::cross(u, v);
    const double k = w.x * a.x + w.y * a.y + w.z * a.z;
    const double dzdx = w.x / (-d * k);
    const double dzdy = w.y / (-d * k);
    const double z1 = (shadows ? 1 : 1.0001) * zg;

    ::Color ambientAndInf = ambient * totalAmbient;
    Vector3D n = w / w.length();
    std::vector<std::pair<double, Vector3D>> cosAndLInf;
    cosAndLInf.reserve(infs.size());
    unsigned int i = 0;
    for (const auto &light: infs) {
        const Vector3D l = -light.direction;
        const double cosA = Vector3D::dot(n, l);
        ambientAndInf += diffuse * light.diffuse * std::max(cosA, 0.0);
        cosAndLInf[i] = {cosA, l};
        i++;
    }

    for (unsigned int y = ymin; y <= ymax; y++) {
        const double z2 = z1 + (y - yg) * dzdy;
        std::vector<double> xL;
        xL.resize(3, DBL_MAX);
        std::vector<double> xR;
        xR.resize(3, -DBL_MAX);
        const std::vector<std::pair<const point *, const point *>> edges = {{&A, &B},
                                                                            {&A, &C},
                                                                            {&B, &C}};
        for (unsigned int j = 0; j < edges.size(); j++) {
            auto P = edges[j].first;
            auto Q = edges[j].second;
            if ((y - P->y) * (y - Q->y) <= 0 && P->y != Q->y) {
                xL[j] = xR[j] = Q->x + (P->x - Q->x) * ((y - Q->y) / (P->y - Q->y));
            }
        }
        const unsigned int xl = static_cast<int>(round(*std::min_element(xL.begin(), xL.end()) + 0.5));
        const unsigned int xr = static_cast<int>(round(*std::max_element(xR.begin(), xR.end()) - 0.5));

        for (unsigned int x = xl; x <= xr; x++) {
            const double z = z2 + (x - xg) * dzdx;
            if (z <= zBuffer[x][y]) {
                const double realX = -(x - dx) / (d * z);
                const double realY = -(y - dy) / (d * z);
                const Vector3D real = Vector3D::point(realX, realY, 1 / z);

                ::Color pointAndSpec;
                for (const auto &light: points) {
                    bool noShadow = true;
                    if (shadows) {
                        const Vector3D shadow = real * Matrix::inv(eye) * light.eye;
                        const double mappedX = (light.d * shadow.x / -shadow.z) + light.dx;
                        const double mappedY = (light.d * shadow.y / -shadow.z) + light.dy;
                        const double alphaX = mappedX - floor(mappedX);
                        const double alphaY = mappedY - floor(mappedY);
                        const double zA = light.shadowMask[floor(mappedX)][ceil(mappedY)];
                        const double zB = light.shadowMask[ceil(mappedX)][ceil(mappedY)];
                        const double zC = light.shadowMask[floor(mappedX)][floor(mappedY)];
                        const double zD = light.shadowMask[ceil(mappedX)][floor(mappedY)];
                        const double zE = (1 - alphaX) * zA + alphaX * zB;
                        const double zF = (1 - alphaX) * zC + alphaX * zD;
                        const double zMask = alphaY * zE + (1 - alphaY) * zF;
                        noShadow = abs(zMask - 1 / shadow.z) < pow(10, -5);
                    }
                    if (noShadow) {
                        Vector3D l = light.location - real;
                        l.normalise();
                        const double cosA = Vector3D::dot(n, l);
                        pointAndSpec += diffuse * light.diffuse * std::max(cosA, 0.0);

                        Vector3D r = 2 * cosA * n - l;
                        const double cosB = Vector3D::dot(r, Vector3D::normalise(Vector3D::vector(-real)));
                        pointAndSpec += specular * light.specular * pow(std::max(cosB, 0.0), coefficient);
                    }
                }

                i = 0;
                for (const auto &light: infs) {
                    const double cosA = cosAndLInf[i].first;
                    const Vector3D l = cosAndLInf[i].second;
                    Vector3D r = 2 * cosA * n - l;
                    const double cosB = Vector3D::dot(r, Vector3D::normalise(Vector3D::vector(-real)));
                    pointAndSpec += specular * light.specular * pow(std::max(cosB, 0.0), coefficient);
                    i++;
                }

                (*this)(x, y) = ambientAndInf + pointAndSpec;
                zBuffer[x][y] = z;
            }
        }
    }
}

void img::EasyImage::draw_textured_triangle(ZBuffer &zBuffer, const Vector3D &a, const Vector3D &b, const Vector3D &c,
                                            double d, double dx, double dy, const img::EasyImage &texture,
                                            const double coefficient, const PointLights &points,
                                            const InfLights &infs, const ::Color &totalAmbient, const Matrix &eye,
                                            const bool shadows, const Vector3D &pTex, const Vector3D &aTex,
                                            const Vector3D &bTex) {
    if ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) < 0)  //backface culling
        return;

    struct point {
        double x;
        double y;
    } const A{(d * a.x / -a.z) + dx, (d * a.y / -a.z) + dy}, B{(d * b.x / -b.z) + dx, (d * b.y / -b.z) + dy}, C{
            (d * c.x / -c.z) + dx, (d * c.y / -c.z) + dy};

    assert(A.x < this->width && A.y < this->height);
    assert(B.x < this->width && B.y < this->height);
    assert(C.x < this->width && C.y < this->height);

    const unsigned int ymin = static_cast<int>(round(std::min(std::min(A.y, B.y), C.y) + 0.5));
    const unsigned int ymax = static_cast<int>(round(std::max(std::max(A.y, B.y), C.y) - 0.5));
    const double xg = (A.x + B.x + C.x) / 3;
    const double yg = (A.y + B.y + C.y) / 3;
    const double zg = 1.0 / (3 * a.z) + 1.0 / (3 * b.z) + 1.0 / (3 * c.z);
    const Vector3D u = b - a;
    const Vector3D v = c - a;
    const Vector3D w = Vector3D::cross(u, v);
    const double k = w.x * a.x + w.y * a.y + w.z * a.z;
    const double dzdx = w.x / (-d * k);
    const double dzdy = w.y / (-d * k);
    const double z1 = (shadows ? 1 : 1.0001) * zg;

    Vector3D cTex = Vector3D::cross(aTex, bTex);
    Matrix abcTex;
    abcTex(1, 1) = aTex.x;
    abcTex(1, 2) = aTex.y;
    abcTex(1, 3) = aTex.z;
    abcTex(2, 1) = bTex.x;
    abcTex(2, 2) = bTex.y;
    abcTex(2, 3) = bTex.z;
    abcTex(3, 1) = cTex.x;
    abcTex(3, 2) = cTex.y;
    abcTex(3, 3) = cTex.z;
    abcTex.inv();

    Vector3D n = w / w.length();
    std::vector<std::pair<double, Vector3D>> cosAndLInf;
    cosAndLInf.reserve(infs.size());
    unsigned int i = 0;
    for (const auto &light: infs) {
        const Vector3D l = -light.direction;
        const double cosA = Vector3D::dot(n, l);
        cosAndLInf[i] = {cosA, l};
        i++;
    }

    for (unsigned int y = ymin; y <= ymax; y++) {
        const double z2 = z1 + (y - yg) * dzdy;
        std::vector<double> xL;
        xL.resize(3, DBL_MAX);
        std::vector<double> xR;
        xR.resize(3, -DBL_MAX);
        const std::vector<std::pair<const point *, const point *>> edges = {{&A, &B},
                                                                            {&A, &C},
                                                                            {&B, &C}};
        for (unsigned int j = 0; j < edges.size(); j++) {
            auto P = edges[j].first;
            auto Q = edges[j].second;
            if ((y - P->y) * (y - Q->y) <= 0 && P->y != Q->y) {
                xL[j] = xR[j] = Q->x + (P->x - Q->x) * ((y - Q->y) / (P->y - Q->y));
            }
        }
        const unsigned int xl = static_cast<int>(round(*std::min_element(xL.begin(), xL.end()) + 0.5));
        const unsigned int xr = static_cast<int>(round(*std::max_element(xR.begin(), xR.end()) - 0.5));

        for (unsigned int x = xl; x <= xr; x++) {
            const double z = z2 + (x - xg) * dzdx;
            if (z <= zBuffer[x][y]) {
                const double realX = -(x - dx) / (d * z);
                const double realY = -(y - dy) / (d * z);
                const Vector3D real = Vector3D::point(realX, realY, 1 / z);

                Vector3D mapped = (real - pTex) * abcTex;
                double xTex = (1 + (texture.get_width() - 1) * mapped.x);
                xTex = fmod(fmod(xTex, texture.width) + texture.width, texture.width);
                double yTex = (1 + (texture.get_height() - 1) * mapped.y);
                yTex = fmod(fmod(yTex, texture.height) + texture.height, texture.height);

                const double deltaX = xTex - floor(xTex);
                const double deltaY = yTex - floor(yTex);
                const Color tempA = texture(int(floor(xTex)) % texture.width, int(ceil(yTex)) % texture.height);
                const Color tempB = texture(int(ceil(xTex)) % texture.width, int(ceil(yTex)) % texture.height);
                const Color tempC = texture(int(floor(xTex)) % texture.width, int(floor(yTex)) % texture.height);
                const Color tempD = texture(int(ceil(xTex)) % texture.width, int(floor(yTex)) % texture.height);
                const ::Color colA(tempA.red / 255.0, tempA.green / 255.0, tempA.blue / 255.0);
                const ::Color colB(tempB.red / 255.0, tempB.green / 255.0, tempB.blue / 255.0);
                const ::Color colC(tempC.red / 255.0, tempC.green / 255.0, tempC.blue / 255.0);
                const ::Color colD(tempD.red / 255.0, tempD.green / 255.0, tempD.blue / 255.0);
                const ::Color colE = colA * (1 - deltaX) + colB * deltaX;
                const ::Color colF = colC * (1 - deltaX) + colD * deltaX;
                const ::Color final = colE * deltaY + colF * (1 - deltaY);

                ::Color pointAndSpec;
                for (const auto &light: points) {
                    bool noShadow = true;
                    if (shadows) {
                        const Vector3D shadow = real * Matrix::inv(eye) * light.eye;
                        const double mappedX = (light.d * shadow.x / -shadow.z) + light.dx;
                        const double mappedY = (light.d * shadow.y / -shadow.z) + light.dy;
                        const double alphaX = mappedX - floor(mappedX);
                        const double alphaY = mappedY - floor(mappedY);
                        const double zA = light.shadowMask[floor(mappedX)][ceil(mappedY)];
                        const double zB = light.shadowMask[ceil(mappedX)][ceil(mappedY)];
                        const double zC = light.shadowMask[floor(mappedX)][floor(mappedY)];
                        const double zD = light.shadowMask[ceil(mappedX)][floor(mappedY)];
                        const double zE = (1 - alphaX) * zA + alphaX * zB;
                        const double zF = (1 - alphaX) * zC + alphaX * zD;
                        const double zMask = alphaY * zE + (1 - alphaY) * zF;
                        noShadow = std::abs(zMask - 1.0 / shadow.z) < pow(10, -5);
                    }
                    if (noShadow) {
                        Vector3D l = light.location - real;
                        l.normalise();
                        const double cosA = Vector3D::dot(n, l);
                        pointAndSpec += final * light.diffuse * std::max(cosA, 0.0);

                        Vector3D r = 2 * cosA * n - l;
                        const double cosB = Vector3D::dot(r, Vector3D::normalise(Vector3D::vector(-real)));
                        pointAndSpec += final * light.specular * pow(std::max(cosB, 0.0), coefficient);
                    }
                }

                ::Color ambientAndInf = final * totalAmbient;
                i = 0;
                for (const auto &light: infs) {
                    const double cosA = cosAndLInf[i].first;
                    const Vector3D l = cosAndLInf[i].second;
                    Vector3D r = 2 * cosA * n - l;
                    const double cosB = Vector3D::dot(r, Vector3D::normalise(Vector3D::vector(-real)));
                    pointAndSpec += final * light.specular * pow(std::max(cosB, 0.0), coefficient);
                    ambientAndInf += final * light.diffuse * std::max(cosA, 0.0);
                    i++;
                }

                (*this)(x, y) = ambientAndInf + pointAndSpec;
                zBuffer[x][y] = z;
            }
        }
    }
}
