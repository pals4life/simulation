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

        while(window.getState() != Window::quit)
        {
            while(window.getState() == Window::play)
            {
                Window::delay(500);
            }
            while(window.getState() == Window::pause)
            {
                Window::delay(10);
            }
            if(window.getState() == Window::next)
            {
                Window::delay(500);
            }
            else if(window.getState() == Window::previous)
            {
                Window::delay(500);
            }
            Window::delay(500);
        }

        delete network;
    }

    application.exit();

    return QApplication::exec();
}
