//============================================================================
// @name        : Main.cpp
// @author      : Thomas Dooms, Ward Gauderis, Mano Marichal
// @date        : 2/28/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Thomas Dooms, Ward Gauderis, Mano Marichal - University of Antwerp
// @description : The main function to the code
//============================================================================


#include "parsers/NetworkParser.h"
#include <iostream>

#include <gtest/gtest.h>
#include "tests/DesignByContract.h"

int main(int argc, char** argv)
{
    REQUIRE(argc == 1, "Argument count must be 1");

	NetworkParser parser;
	parser.loadFile("inputfiles/spec1.0.xml");
	parser.parseNetwork(parser.getRoot());
	Network *network = parser.getNetwork();
	parser.clear();

	network->startSimulation();
	std::cout << "simulation ended in: " << network->getTicksPassed() << " ticks\n";
	delete network;

	return 0;
}