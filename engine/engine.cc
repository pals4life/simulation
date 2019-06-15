#include "easy_image.h"
#include "ini_configuration.h"
#include "Line2D.h"
#include "Light.h"
#include <fstream>

enum render {
    wire, zbuf, triangle, lighted
};

img::EasyImage introColorRectangle(const ini::Configuration &configuration) {
    int width = configuration["ImageProperties"]["width"];
    int height = configuration["ImageProperties"]["height"];
    img::EasyImage image(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j)).red = i;
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j)).green = j;
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j)).blue = (i + j) % 256;
        }
    }
    return image;
}

img::EasyImage introBlocks(const ini::Configuration &configuration) {
    int width = configuration["ImageProperties"]["width"];
    int height = configuration["ImageProperties"]["height"];
    img::EasyImage image(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    int nrXBlocks = configuration["BlockProperties"]["nrXBlocks"];
    int nrYBlocks = configuration["BlockProperties"]["nrYBlocks"];
    std::vector<double> colorWhite = configuration["BlockProperties"]["colorWhite"];
    std::vector<double> colorBlack = configuration["BlockProperties"]["colorBlack"];
    Color white = colorWhite;
    Color black = colorBlack;
    bool invertColors = configuration["BlockProperties"]["invertColors"];
    int blockWidth = static_cast<int>(round(double(width) / nrXBlocks));
    int blockHeight = static_cast<int>(round(double(width) / nrYBlocks));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; ++y) {
            int xCo = x / blockWidth;
            int yCo = y / blockHeight;
            if ((xCo + yCo + invertColors) % 2 == 0) {
                image(static_cast<unsigned int>(x), static_cast<unsigned int>(y)) = white;
            } else {
                image(static_cast<unsigned int>(x), static_cast<unsigned int>(y)) = black;
            }
        }
    }
    return image;
}

void quadLines(img::EasyImage &image, unsigned int nrLines, unsigned int height, unsigned int width,
               const Color &line, unsigned int lineY, unsigned int lineX) {
    auto deltaH = static_cast<unsigned int>(round(height / (nrLines - 1.0)));
    auto deltaW = static_cast<unsigned int>(round(width / (nrLines - 1.0)));
    unsigned int x = 0;
    unsigned int y = 0;
    for (unsigned int i = 0; i < nrLines; ++i) {
        image.draw_line(x, lineY, lineX, y, line);
        x += deltaW;
        y += deltaH;
    }
}

img::EasyImage introLines(const ini::Configuration &configuration) {
    int width = configuration["ImageProperties"]["width"];
    int height = configuration["ImageProperties"]["height"];
    img::EasyImage image(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    std::string figure = configuration["LineProperties"]["figure"];
    int nrLines = configuration["LineProperties"]["nrLines"];
    std::vector<double> backgroundcolor = configuration["LineProperties"]["backgroundcolor"];
    std::vector<double> lineColor = configuration["LineProperties"]["lineColor"];
    Color background = backgroundcolor;
    Color line = lineColor;
    image.clear(background);
    if (figure == "QuarterCircle") {
        quadLines(image, static_cast<unsigned int>(nrLines), static_cast<unsigned int>(height),
                  static_cast<unsigned int>(width), line, static_cast<unsigned int>(height - 1), 0);
    } else if (figure == "Eye") {
        quadLines(image, static_cast<unsigned int>(nrLines), static_cast<unsigned int>(height),
                  static_cast<unsigned int>(width), line, static_cast<unsigned int>(height - 1), 0);
        quadLines(image, static_cast<unsigned int>(nrLines), static_cast<unsigned int>(height),
                  static_cast<unsigned int>(width), line, 0, static_cast<unsigned int>(width - 1));
    } else if (figure == "Diamond") {
        auto deltaH = static_cast<unsigned int>(round(height / (2 * (nrLines - 1.0))));
        auto deltaW = static_cast<unsigned int>(round(width / (2 * (nrLines - 1.0))));
        for (unsigned int i = 0; i < static_cast<unsigned int>(nrLines); ++i) {
            image.draw_line((width / 2) + i * deltaW, static_cast<unsigned int>(height / 2),
                            static_cast<unsigned int>(width / 2),
                            (height / 2) + ((nrLines - 1 - i) * deltaH), line);
            image.draw_line((width / 2) + i * deltaW, static_cast<unsigned int>(height / 2),
                            static_cast<unsigned int>(width / 2),
                            (height / 2) - ((nrLines - 1 - i) * deltaH), line);
            image.draw_line((width / 2) - ((nrLines - 1 - i) * deltaW), static_cast<unsigned int>(height / 2),
                            static_cast<unsigned int>(width / 2),
                            (height / 2) + i * deltaH, line);
            image.draw_line((width / 2) - ((nrLines - 1 - i) * deltaW), static_cast<unsigned int>(height / 2),
                            static_cast<unsigned int>(width / 2),
                            (height / 2) - i * deltaH, line);
        }
    }
    return image;
}

img::EasyImage lSystem2D(const ini::Configuration &configuration) {
    const std::string inputfile = configuration["2DLSystem"]["inputfile"];
    const int size = configuration["General"]["size"];
    std::vector<double> backgroundcolor = configuration["General"]["backgroundcolor"];
    std::vector<double> linecolor = configuration["2DLSystem"]["color"];
    Color background = backgroundcolor;
    Color line = linecolor;
    LParser::LSystem2D lSystem2D;
    std::ifstream inputStream(inputfile);
    inputStream >> lSystem2D;
    inputStream.close();
    Lines2D lines{lSystem2D, line};
    return lines.draw((unsigned int) size, background, false);
}

void getFigure(const ini::Configuration &configuration, Figures &figures, const std::string &name, const Color &ambient,
               const Color &diffuse, const Color &specular, const double coefficient, const std::string &texture,
               const Vector3D &p, const Vector3D &a, const Vector3D &b) {
    std::string figureType = configuration[name]["type"];
    double scale = configuration[name]["scale"];
    double x = M_PI * configuration[name]["rotateX"].as_double_or_default(0) / 180;
    double y = M_PI * configuration[name]["rotateY"].as_double_or_default(0) / 180;
    double z = M_PI * configuration[name]["rotateZ"].as_double_or_default(0) / 180;
    std::vector<double> centerPoint = configuration[name]["center"];
    Vector3D center = Vector3D::point(centerPoint);
    Figure figure;

    if (figureType == "LineDrawing") {
        figure = Figure(name, configuration);
    } else if (figureType == "Cube") {
        figure = Figure::cube();
    } else if (figureType == "Tetrahedron") {
        figure = Figure::tetrahedron();
    } else if (figureType == "Octahedron") {
        figure = Figure::octahedron();
    } else if (figureType == "Icosahedron") {
        figure = Figure::icosahedron();
    } else if (figureType == "BuckyBall") {
        figure = Figure::buckyball();
    } else if (figureType == "Dodecahedron") {
        figure = Figure::dodecahedron();
    } else if (figureType == "Cylinder") {
        const int n = configuration[name]["n"];
        const double height = configuration[name]["height"];
        figure = Figure::cylinder(n, height, true);
    } else if (figureType == "Cone") {
        const int n = configuration[name]["n"];
        const double height = configuration[name]["height"];
        figure = Figure::cone(n, height);
    } else if (figureType == "Sphere") {
        const int n = configuration[name]["n"];
        figure = Figure::sphere(n);
    } else if (figureType == "Torus") {
        const double R = configuration[name]["R"];
        const double r = configuration[name]["r"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        figure = Figure::Torus(R, r, n, m);
    } else if (figureType == "3DLSystem") {
        const std::string inputfile = configuration[name]["inputfile"];
        LParser::LSystem3D lSystem3D;
        std::ifstream inputStream(inputfile);
        inputStream >> lSystem3D;
        inputStream.close();
        figure = lSystem3D;
    } else if (figureType == "FractalCube") {
        const int nrIterations = configuration[name]["nrIterations"];
        const double fractalScale = configuration[name]["fractalScale"];
        Figure temp = Figure::cube();
        temp *= scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures::fractal(temp, nrIterations, fractalScale);
    } else if (figureType == "FractalTetrahedron") {
        const int nrIterations = configuration[name]["nrIterations"];
        const double fractalScale = configuration[name]["fractalScale"];
        Figure temp = Figure::tetrahedron();
        temp *= scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures::fractal(temp, nrIterations, fractalScale);
    } else if (figureType == "FractalOctahedron") {
        const int nrIterations = configuration[name]["nrIterations"];
        const double fractalScale = configuration[name]["fractalScale"];
        Figure temp = Figure::octahedron();
        temp *= scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures::fractal(temp, nrIterations, fractalScale);
    } else if (figureType == "FractalIcosahedron") {
        const int nrIterations = configuration[name]["nrIterations"];
        const double fractalScale = configuration[name]["fractalScale"];
        Figure temp = Figure::icosahedron();
        temp *= scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures::fractal(temp, nrIterations, fractalScale);
    } else if (figureType == "FractalBuckyBall") {
        const int nrIterations = configuration[name]["nrIterations"];
        const double fractalScale = configuration[name]["fractalScale"];
        Figure temp = Figure::buckyball();
        temp *= scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures::fractal(temp, nrIterations, fractalScale);
    } else if (figureType == "FractalDodecahedron") {
        const int nrIterations = configuration[name]["nrIterations"];
        const double fractalScale = configuration[name]["fractalScale"];
        Figure temp = Figure::dodecahedron();
        temp *= scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures::fractal(temp, nrIterations, fractalScale);
    } else if (figureType == "MengerSponge") {
        const int nrIterations = configuration[name]["nrIterations"];
        Figures temp =
                Figures::mengerSponge(nrIterations) * scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) *
                translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += std::move(temp);
    } else if (figureType == "ThickLineDrawing") {
        const double r = configuration[name]["radius"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        Figure temp = Figure(name, configuration);
        temp *= scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures(temp, r, n, m);
    } else if (figureType == "ThickCube") {
        const double r = configuration[name]["radius"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        Figure temp = Figure::cube();
        temp *= rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures(temp, r, n, m) * scaleFigure(scale);
    } else if (figureType == "ThickDodecahedron") {
        const double r = configuration[name]["radius"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        Figure temp = Figure::dodecahedron();
        temp *= rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures(temp, r, n, m) * scaleFigure(scale);
    } else if (figureType == "ThickIcosahedron") {
        const double r = configuration[name]["radius"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        Figure temp = Figure::icosahedron();
        temp *= rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures(temp, r, n, m) * scaleFigure(scale);
    } else if (figureType == "ThickOctahedron") {
        const double r = configuration[name]["radius"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        Figure temp = Figure::octahedron();
        temp *= rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures(temp, r, n, m) * scaleFigure(scale);
    } else if (figureType == "ThickTetrahedron") {
        const double r = configuration[name]["radius"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        Figure temp = Figure::tetrahedron();
        temp *= rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures(temp, r, n, m) * scaleFigure(scale);
    } else if (figureType == "Thick3DLSystem") {
        const double r = configuration[name]["radius"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        const std::string inputfile = configuration[name]["inputfile"];
        LParser::LSystem3D lSystem3D;
        std::ifstream inputStream(inputfile);
        inputStream >> lSystem3D;
        inputStream.close();
        Figure temp = lSystem3D;
        temp *= rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures(temp, r, n, m) * scaleFigure(scale);
    } else if (figureType == "ThickBuckyBall") {
        const double r = configuration[name]["radius"];
        const int n = configuration[name]["n"];
        const int m = configuration[name]["m"];
        Figure temp = Figure::buckyball();
        temp *= rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        temp.setColor(ambient, diffuse, specular, coefficient);
        temp.setTexture(texture, p, a, b);
        figures += Figures(temp, r, n, m) * scaleFigure(scale);
    }

    if (!figure.getPoints().empty()) {
        figure *= scaleFigure(scale) * rotateX(x) * rotateY(y) * rotateZ(z) * translate(center);
        figure.setColor(ambient, diffuse, specular, coefficient);
        figure.setTexture(texture, p, a, b);
        figures.addFigure(std::move(figure));
    }
}

img::EasyImage draw3D(const ini::Configuration &configuration, const render type) {
    const int size = configuration["General"]["size"];
    const std::vector<double> background = configuration["General"]["backgroundcolor"];
    const int nrFigures = configuration["General"]["nrFigures"].as_int_or_default(0);
    const int nrLights = configuration["General"]["nrLights"].as_int_or_default(0);
    const std::vector<double> eyeP = configuration["General"]["eye"];
    const Matrix eye = eyePoint(Vector3D::point(eyeP));
    const bool shadowEnabled = configuration["General"]["shadowEnabled"].as_bool_or_default(false);
    const int shadowMask = configuration["General"]["shadowMask"].as_int_or_default(0);

    Figures figures;
    for (int i = nrFigures - 1; i >= 0; --i) {
        const std::string name = "Figure" + std::to_string(i);
        std::vector<double> ambient = {0, 0, 0};
        std::vector<double> diffuse = {0, 0, 0};
        std::vector<double> specular = {0, 0, 0};
        double coefficient = 0;
        if (type == lighted) {
            if (configuration[name]["ambientReflection"].exists())
                ambient = configuration[name]["ambientReflection"];
            if (configuration[name]["diffuseReflection"].exists())
                diffuse = configuration[name]["diffuseReflection"];
            if (configuration[name]["specularReflection"].exists())
                specular = configuration[name]["specularReflection"];
            if (configuration[name]["reflectionCoefficient"].exists())
                coefficient = configuration[name]["reflectionCoefficient"];
        } else {
            ambient = configuration[name]["color"];
        }

        std::vector<double> p = {0, 0, 0};
        std::vector<double> a = {0, 0, 0};
        std::vector<double> b = {0, 0, 0};
        std::string texture;
        const bool textured = configuration[name]["texture"].exists();
        if (textured) {
            p = configuration[name]["p"];
            a = configuration[name]["a"];
            b = configuration[name]["b"];
            texture = configuration[name]["texture"].as_string_or_default("");
        }

        getFigure(configuration, figures, name, ambient, diffuse, specular, coefficient, texture, Vector3D::point(p),
                  Vector3D::vector(a), Vector3D::vector(b));
    }

    PointLights points;
    InfLights infs;
    if (type == lighted) {
        for (int i = 0; i < nrLights; ++i) {
            const std::string name = "Light" + std::to_string(i);
            const bool infinity = configuration[name]["infinity"].as_bool_or_default(true);
            std::vector<double> ambient = {0, 0, 0};
            if (configuration[name]["ambientLight"].exists()) ambient = configuration[name]["ambientLight"];
            std::vector<double> diffuse = {0, 0, 0};
            if (configuration[name]["diffuseLight"].exists()) diffuse = configuration[name]["diffuseLight"];
            std::vector<double> specular = {0, 0, 0};
            if (configuration[name]["specularLight"].exists()) specular = configuration[name]["specularLight"];
            if (infinity) {
                std::vector<double> direction = {1, 1, 1};
                if (configuration[name]["direction"].exists()) direction = configuration[name]["direction"];
                infs.emplace_back(ambient, diffuse, specular, Vector3D::vector(direction));
            } else {
                std::vector<double> location = {0, 0, 0};
                if (configuration[name]["location"].exists()) location = configuration[name]["location"];
                points.emplace_back(ambient, diffuse, specular, Vector3D::point(location));
            }
        }
    } else {
        infs.emplace_back(Color(1, 1, 1), Color(), Color(), Vector3D::vector(1, 1, 1));
    }

    if (type == wire) {
        figures *= eye;
        Lines2D lines = figures;
        return lines.draw((unsigned int) size, background, false);
    } else if (type == zbuf) {
        figures *= eye;
        Lines2D lines = figures;
        return lines.draw((unsigned int) size, background, true);
    } else if (type == triangle or type == lighted) {
        figures.triangulate();
        if (shadowEnabled) figures.generateShadowMasks(points, shadowMask);
        figures *= eye;
        points *= eye;
        infs *= eye;
        return figures.draw((unsigned int) size, background, points, infs, eye, shadowEnabled);
    }
    return img::EasyImage();
}

img::EasyImage generate_image(const ini::Configuration &configuration) {
    std::string type = configuration["General"]["type"].as_string_or_die();
    if (type == "IntroColorRectangle") {
        return introColorRectangle(configuration);
    } else if (type == "IntroBlocks") {
        return introBlocks(configuration);
    } else if (type == "IntroLines") {
        return introLines(configuration);
    } else if (type == "2DLSystem") {
        return lSystem2D(configuration);
    } else if (type == "Wireframe") {
        return draw3D(configuration, wire);
    } else if (type == "ZBufferedWireframe") {
        return draw3D(configuration, zbuf);
    } else if (type == "ZBuffering") {
        return draw3D(configuration, triangle);
    } else if (type == "LightedZBuffering") {
        return draw3D(configuration, lighted);
    }
    return img::EasyImage();
}

int main(int argc, char const *argv[]) {
    int retVal = 0;
    try {
        for (int i = 1; i < argc; ++i) {
            ini::Configuration conf;
            try {
                std::ifstream fin(argv[i]);
                fin >> conf;
                fin.close();
            }
            catch (ini::ParseException &ex) {
                std::cerr << "Error parsing file: " << argv[i] << ": " << ex.what() << std::endl;
                retVal = 1;
                continue;
            }

            img::EasyImage image = generate_image(conf);
            if (image.get_height() > 0 && image.get_width() > 0) {
                std::string fileName(argv[i]);
                std::string::size_type pos = fileName.rfind('.');
                if (pos == std::string::npos) {
                    //filename does not contain a '.' --> append a '.bmp' suffix
                    fileName += ".bmp";
                } else {
                    fileName = fileName.substr(0, pos) + ".bmp";
                }
                try {
                    std::ofstream f_out(fileName.c_str(), std::ios::trunc | std::ios::out | std::ios::binary);
                    f_out << image;

                }
                catch (std::exception &ex) {
                    std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                    retVal = 1;
                }
            } else {
                std::cout << "Could not generate image for " << argv[i] << std::endl;
            }
        }
    }
    catch (const std::bad_alloc &exception) {
        //When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
        //Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
        //(Unless of course you are already consuming the maximum allowed amount of memory)
        //If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
        //mark the test as failed while in reality it just needed a bit more memory
        std::cerr << "Error: insufficient memory" << std::endl;
        retVal = 100;
    }
    return retVal;
}
