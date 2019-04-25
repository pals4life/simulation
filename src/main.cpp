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

int main(int argc, char **argv) {

    REQUIRE(argc == 2, "argument count must be 1");

    QApplication application(argc, argv);

    Window window;
    window.createButtons();
    window.init();

    NetworkParser parser;
    if (parser.loadFile(argv[1])) {
        Network* network = parser.parseNetwork(parser.getRoot());
        parser.clear();

        network->startSimulation(false, true);
        delete network;
    }

    QApplication::exit();
    return QApplication::exec();
}
