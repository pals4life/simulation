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
#include <QApplication>
#include "gui/gui.h"
#include "gtest/gtest.h"
#include "DesignByContract.h"

int main(int argc, char** argv)
{
    QApplication application(argc, argv);

    Window* window = nullptr;
    std::string filename;

    window = new Window;
    window->init();
    window->createButtons();

    if(argc != 2) filename = window->askString("inputfiles/spec2.0.xml");
    else filename = argv[1];

    NetworkParser parser;
    if (parser.loadFile(filename))
    {
        Network* network = parser.parseNetwork(parser.getRoot());
        parser.clear();

        window->createRoadButtons(network->getRoads());

        network->startSimulation(window, "simple", "impression");
        delete network;
    }

    QApplication::exit();
    return QApplication::exec();
}
