//============================================================================
// @name        : NetworkExporter.h
// @author      : Thomas Dooms, Ward Gauderis
// @date        : 3/26/19
// @version     : 2.0
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : Exports network to ofstreams
//============================================================================


#ifndef SIMULATION_NETWORKEXPORTER_H
#define SIMULATION_NETWORKEXPORTER_H

#include "../datatypes/Network.h"
#include <fstream>
#include <stdint.h>
#include <ostream>

struct Color {
    double fR;
    double fG;
    double fB;

    bool properlyInitialized() const;

    friend std::ostream &operator<<(std::ostream &os, const Color &color);

    Color(double fR, double fG, double fB);

    Color();

private:
    Color *_initCheck;
};

struct Pos {
    double fX;
    double fY;
    double fZ;

    bool properlyInitialized() const;

    friend std::ostream &operator<<(std::ostream &os, const Pos &pos);

    Pos(double fX, double fY, double fZ);

private:
    Pos *_initCheck;
};

struct Face {
    std::vector<int> fIndexes;

    Face(std::vector<int> fIndexes);

    friend std::ostream &operator<<(std::ostream &os, const Face &face);

    bool properlyInitialized() const;

private:
    Face *_initCheck;
};

struct Object {
    std::vector<Pos> fPoints;
    std::vector<Face> fFaces;
    Color fAmbient;
    Color fDiffuse;
    Color fSpecular;
    double fReflectionCoefficient;

    bool properlyInitialized() const;

    static Object rectangle(const Pos &begin, const Pos &end);

    void print(std::ofstream &ini, int nr);

    Object();

private:
    Object *_initCheck;
};

class NetworkExporter {
public:

    /**
     *  REQUIRE(kNetwork, "Failed to export network: no network");
     *  ENSURE(res == 0 or res == 256, "Failed to create output directory");
     *  ENSURE(fgSimple.is_open(), "Failed to load file for simple output");
     *  ENSURE(fgImpression.is_open(), "Failed to load File for impression output");
     */
    static void init(const Network *network, const std::string &kSimplePath, const std::string &kImpressionPath);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling addSection");
     *  REQUIRE(kNetwork, "Failed to add section: no network");
     */
    static std::string addSection(const Network *kNetwork, uint32_t number);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling finish");
     */
    static void finish();

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling tee");
     */
    template<typename T>
    static void tee(const T &string, bool init) {
        REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling tee");
        fgImpression << string;
        std::cout << string;
        fgBuf << string;
        if (init) fgSimple << string;
    }

    static std::string whitespace(int amount);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling printLane");
     */
    static void printLane(const std::vector<std::vector<char>> &lane, uint32_t max, uint32_t laneNum);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling cgExport");
     *  REQUIRE(kNetwork, "Failed to export to cg: no network");
     *  REQUIRE(FileExists("engine/engine"), "Failed to export to cg engine: engine not found");
     *  ENSURE(res == 0 or res == 256, "Failed to create output directory");
     *  ENSURE(ini.is_open(), "Failed to open file for cg export");
     *  ENSURE(res == 0, "Failed to run engine on the generated ini file");
     *  ENSURE(!ini.is_open(), "Failed to close ofstream to ini");
     */
    static void cgExport(const Network *kNetwork, unsigned int kTick);

    static bool properlyInitialized();

private:
    static std::ofstream fgSimple;
    static std::ofstream fgImpression;

    static std::stringstream fgBuf;

    static double fgScale;
    static uint32_t fgLongestName;

    static bool _initCheck;

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling sign");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void sign(std::ofstream &ini, int &nr, const double &x, const double &y, char c);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling general");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void general(std::ofstream &ini, const int &kNr);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling line");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void line(std::ofstream &ini, int &nr, const double &y, const double &x);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling lane");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void lane(std::ofstream &ini, int &nr, double max, double y, double roadlength);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling wheel");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void wheel(std::ofstream &ini, int &nr, const Pos &kPos);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling car");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void car(std::ofstream &ini, int &nr, const Pos &pos, bool real);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling bus");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void bus(std::ofstream &ini, int &nr, const Pos &pos);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling truck");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void truck(std::ofstream &ini, int &nr, const Pos &pos);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling motorcycle");
     *  REQUIRE(ini.is_open(), "Ofstream to ini is not open");
     *  REQUIRE(nr >= 0, "Nr must be greater than 0");
     */
    static void motorcycle(std::ofstream &ini, int &nr, const Pos &pos);

};

// source: Serge Demeyer - TicTactToe in C++, Ansi-style
bool FileExists(const std::string &filename);

#endif //SIMULATION_NETWORKEXPORTER_H
