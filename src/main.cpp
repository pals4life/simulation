//============================================================================
// @name        : Main.cpp
// @author      : Thomas Dooms, Ward Gauderis, Mano Marichal
// @date        : 2/28/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Thomas Dooms, Ward Gauderis, Mano Marichal - University of Antwerp
// @description : The main function to the code
//============================================================================


#include "parsers/NetworkParser.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
    NetworkParser parser;
    std::ifstream file("inputfiles/spec1.0.xml");
    file >> parser;
    Network* network = parser.getNetwork();
    network->startSimulation();
    std::cout << "simulation ended in: " << network->getTicksPassed() << " ticks\n";
    return 0;
}