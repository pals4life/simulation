//============================================================================
// @name        : NetworkExporter.cpp
// @author      : Thomas Dooms
// @date        : 3/26/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "NetworkExporter.h"
#include "../DesignByContract.h"
#include <stdlib.h>
#include <math.h>

std::ofstream NetworkExporter::fSimple;
std::ofstream NetworkExporter::fImpression;

double NetworkExporter::scale = 0;
uint32_t NetworkExporter::longestName = 0;


void NetworkExporter::init(const Network* kNetwork, const std::string &kSimplePath, const std::string &kImpressionPath)
{
    int res = system("mkdir outputfiles >/dev/null 2>&1");
    ENSURE(res == 0 or res == 256, "could not make directory");
    fSimple.open(("outputfiles/" + kSimplePath + ".txt").c_str());
    ENSURE(fSimple.is_open(), "output file is not open");

    fImpression.open(("outputfiles/" + kImpressionPath + ".txt").c_str());
    ENSURE(fImpression.is_open(), "output file is not open");

    double maxLength = 0;
    for(uint32_t i = 0; i < kNetwork->fRoads.size(); i++)
    {
        const Road* road = kNetwork->fRoads[i];

        fSimple << "Baan : " + road->getName() + '\n';
        fSimple << "  -> snelheidslimiet: "  << road->getSpeedLimit() * 3.6 << '\n';
        fSimple << "  -> lengte         : "  << road->getRoadLength()       << '\n';

        tee("Baan : " + road->getName() + '\n');
        tee("  -> snelheidslimiet: " + std::to_string(road->getSpeedLimit() * 3.6) + '\n');
        tee("  -> lengte         : " + std::to_string(road->getRoadLength()) + '\n');

        if (road->getRoadLength() > maxLength) maxLength = road->getRoadLength();
        if (road->getName().size() > longestName) longestName = road->getName().size();
    }
    scale = maxLength / 120;
    tee("\n-------------------------------------------------\nOne character is " + std::to_string(scale) + " meters");
}

void NetworkExporter::finish()
{
    fSimple << std::flush;
    fSimple.close();
}

void NetworkExporter::addSection(const Network* kNetwork, uint32_t number)
{
    fSimple << "\n-------------------------------------------------\n";
    if(number != 1) fSimple << "State of the network after " << number << " ticks have passed:\n\n";
    else fSimple << "State of the network after " << number << " tick has passed:\n\n";
    for(uint32_t i = 0; i < kNetwork->fRoads.size(); i++)
    {
        const Road* road = kNetwork->fRoads[i];
        for(uint32_t j = 0; j < road->getNumLanes(); j++)
        {
            for(uint32_t k = 0; k < (*road)[j].size(); k++)
            {
                const IVehicle* vehicle = (*road)[j][k];
                fSimple << "Voertuig: " + vehicle->getType() +'('+ vehicle->getLicensePlate() + ")\n";
                fSimple << "  -> Baan    : " << road->getName()            << '\n';
                fSimple << "  -> Positie : " << vehicle->getPosition()     << '\n';
                fSimple << "  -> Snelheid: " << vehicle->getVelocity()*3.6 << '\n';
            }
        }
    }

    tee("\n-------------------------------------------------\n");
    if (number != 1) tee("State of the network after " + std::to_string(number) + " ticks have passed:\n\n");
    else tee("State of the network after " + std::to_string(number) + " tick has passed:\n\n");
    for (uint32_t i = 0; i < kNetwork->fRoads.size(); i++) {
        const Road *road = kNetwork->fRoads[i];
        tee(road->getName() + whitespace(longestName - road->getName().size()) + " | ");
        for (uint32_t j = 0; j < road->getNumLanes(); j++) {
            std::vector<std::vector<char> > lane;
            uint32_t max = 0;
            lane.resize(static_cast<uint32_t >(floor(road->getRoadLength()/scale)+1));
            for (uint32_t k = 0; k < (*road)[j].size(); k++) {
                const IVehicle *vehicle = (*road)[j][k];
                uint32_t pos = static_cast<uint32_t >(floor(vehicle->getPosition() / scale));
                lane[pos].push_back(toupper(vehicle->getType()[0]));
                if (lane[pos].size() > max) max = lane[pos].size();
            }
            if (max == 0) {
                printLane(lane, 1, j);
            } else {
                printLane(lane, max, j);
            }
        }
    }
}

std::string NetworkExporter::whitespace(const int amount)
{
    std::string white;
    for (int i = 0; i < amount; ++i) {
        white+=" ";
    }
    return white;
}

void NetworkExporter::printLane(const std::vector<std::vector<char>> &lane, const uint32_t max, const uint32_t laneNum)
{
    for (uint32_t l = 0; l < max; ++l) {
        if (laneNum != 0) tee(whitespace(longestName + 3));
        if (l == 0) tee(std::to_string(laneNum + 1) + " ");
        else tee(whitespace(longestName + 4 + std::to_string(laneNum + 1).size()));
        for (uint32_t k = 0; k < lane.size(); ++k) {
            tee((l < lane[k].size() ? lane[k][l] : (l == 0 ? '=' : ' ')));
        }
        tee('\n');
    }
}

template<class T>
void NetworkExporter::tee(const T&string) {
    fImpression << string;
    std::cout << string;
}
