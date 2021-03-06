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

    Window* window = new Window;
    std::string filename;

    const bool GUI = true;

    if (GUI)
    {
        window->init();
        window->createButtons();
    }

    if (argc > 1) filename = argv[1];
    else if(GUI) filename = window->askString("inputfiles/use_case_test_files/use_case_3.1.xml");
    else throw std::runtime_error("argument count must be > 1, if gui is false");

    NetworkParser parser;
    if (parser.loadFile(filename))
    {
        Network* network = parser.parseNetwork(parser.getRoot());
        parser.clear();

        if (GUI)
        {
            window->createRoadButtons(network->getRoads());
        }
        network->startSimulation(window, "simple", "impression", !GUI);
        delete network;
    }

    if (GUI)
    {
        QApplication::exit();
        return QApplication::exec();
    }
    return 0;
}
