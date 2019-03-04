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
	NetworkParser parser;
	parser.loadFile("inputfiles/spec1.0.xml");
	parser.parseNetwork(parser.getRoot());
	Network *network = parser.getNetwork();
	parser.clear();
	network->startSimulation(1000);
	std::cout << "simulation ended in: " << network->getTicksPassed() << " ticks\n";
	delete network;
	return 0;
}