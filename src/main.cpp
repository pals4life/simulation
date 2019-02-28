//============================================================================
// @name        : Main.cpp
// @author      : Thomas Dooms, Ward Gauderis, Mano Marichal
// @date        : 2/28/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Thomas Dooms, Ward Gauderis, Mano Marichal - University of Antwerp
// @description : The main function to the code
//============================================================================


#include "Parsers/NetworkParser.h"
#include <fstream>
#include <iostream>
#include "Road.h"
#include <algorithm>

int main(int argc, char** argv)
{
//    NetworkParser parser;
//    std::ifstream file("inputfiles/spec1.0.xml");
//    file >> parser;
//    Network* network = parser.getNetwork();
//    network->startSimulation();
//    std::cout << "simulation ended in: " << network->getTicksPassed() << " ticks\n";
//
//    delete network;
//    return 0;

//    little test for Ward
    std::vector<Road*> roads;
    roads.push_back(new Road("name0", NULL, 1000, 100));
    roads.push_back(new Road("name1", NULL, 1000, 200));
    roads.push_back(new Road("name2", NULL, 1000, 300));

    std::string name = "name0";
    std::vector<Road*>::iterator a = std::find(roads.begin(), roads.end(), name);

    if(a == roads.end()) std::cerr << "aaaaaaaaargh\n";
    else std::cout << (*a)->getSpeedLimit() << '\n';

    return 0;
}