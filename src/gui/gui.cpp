//============================================================================
// @name        : gui.cpp
// @author      : Mano Marichal
// @date        : 
// @version     : 
// @copyright   : BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================

#include <QtCore/QTime>
#include "gui.h"

//--------------------------WINDOW CLASS----------------------------------------

Window::Window(QWidget *parent) : QMainWindow(parent)
{

}

void Window::init()
{
    this->setWindowTitle("Simulation");
    this->setCentralWidget(root);
    this->show();
    root->setLayout(layout);

    QLabel* title = new QLabel("Project Software Engineering - BA1 Informatica - Thomas Dooms, Ward Gauderis, Mano Marichal - University of Antwerp");
    layout->addWidget(title, 0,0,1,3);

    properlyInitialized = true;

    ENSURE(this->checkProperlyInitialized(), "Window.init() must end in properlyInitialized state");
}

void Window::delay(uint32_t ms)
{
    QTime stopTime = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < stopTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        system("sleep 0.01");
    }
}

void Window::createButtons()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling CreateButtons");

    QPushButton* play = new QPushButton("Play", this);
    QPushButton* pause = new QPushButton("Pause", this);
    QPushButton* skipOne = new QPushButton("Next tick", this);

    play->show();
    pause->show();
    skipOne->show();

    int size = 70;

    play->setFixedHeight(size);
    pause->setFixedHeight(size);
    skipOne->setFixedHeight(size);

    layout-> addWidget(play, 1, 0 ,1, 1);
    layout-> addWidget(pause, 1, 1 ,1, 1);
    layout-> addWidget(skipOne, 1, 2 ,1, 1);

    connect(play, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(pause, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(skipOne, SIGNAL(pressed()), this, SLOT(onNext()));
}

std::string Window::askString()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling askString");

    bool ok;
    QString text = QInputDialog::getText(this, tr("enter your filename"),
                                         tr("filename"), QLineEdit::Normal,
                                         QString("inputfiles/spec1.0.xml"), &ok);

    if (ok && !text.isEmpty())
        return text.toStdString();
    else return "";
}

double Window::askDouble()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling askDouble");

    bool ok;
    double val = QInputDialog::getDouble(this, tr(""),
                                       tr("New value:"), 120, 0, 10000, 1, &ok);
    if (ok)
    {
        return val;
    }
    else
    {
        return -1;
    }
}

void Window::createRoadButtons(const std::vector<Road *> &roads)
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling CreateButtons");

    for (unsigned int i=0;i<roads.size();i++)
    {
        QPushButton* temp = new QPushButton( roads[i]->getName().c_str(), this);
        temp->show();
        connect(temp, SIGNAL(pressed()), this, SLOT(onRoadButton()));

        layout->addWidget(temp, i+2, 0, 1, 3);
        fRoadButtons[temp] = roads[i];
    }
}

Window::state Window::getState() const
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling CreateButtons");

    state temp = crState;
    if(crState == next) crState = pause;
    return temp;
}

void Window::closeEvent (QCloseEvent *event)
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling CreateButtons");

    crState = quit;
}

bool Window::checkProperlyInitialized() const
{
    return properlyInitialized;
}

void Window::processEvents()
{
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

// private slot functions

void Window::onPlay()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onPlay");

    crState = play;
}

void Window::onPause()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onPause");

    crState = pause;
}

void Window::onNext()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onNext");

    crState = next;
}
void Window::onExit()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onExit");

    crState = quit;
}
void Window::onRoadButton()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onRoadButton");

    QObject* obj = sender();

    RoadWindow window;
    window.setRoad(fRoadButtons[obj]);
    window.init();
    while (window.crState != quit) Window::delay(500);
}

//---------------------------------------ROAD WINDOW CLASS-----------------------------------------------------
void RoadWindow::init()
{
    REQUIRE(fRoad != NULL, "roadwindow has no road");

    this->setWindowTitle(fRoad->getName().c_str());
    this->setCentralWidget(root);
    this->show();

    root->setLayout(layout);
    root->show();

    QLabel* name = new QLabel(("Name: " + fRoad->getName()).c_str());
    layout->addWidget(name, 0,0,1,1);

    QLabel* speedlimit = new QLabel(("Speedlimit: " + std::to_string(fRoad->getSpeedLimit())).c_str());
    layout->addWidget(speedlimit, 1,0,1,1);

    QPushButton *editSpdLimit = new QPushButton("Edit");
    layout->addWidget(editSpdLimit, 1, 1, 1, 1);
    connect(editSpdLimit, SIGNAL(pressed()), this, SLOT(onEditSpeedLimit()));


    QLabel* length = new QLabel(("Length: " + std::to_string(fRoad->getRoadLength())).c_str());
    layout->addWidget(length, 2,0,1,1);

    std::string next;
    if (fRoad->getNextRoad() == NULL)
    {
        next = "this road has no next road";
    }
    else next = fRoad->getNextRoad()->getName();

    QLabel* nextinf = new QLabel(("Next Road: " + next).c_str());
    layout->addWidget(nextinf, 3,0,1,1);

    QPushButton *exit = new QPushButton("Save changes and exit");
    layout->addWidget(exit, 4, 0, 1, 2);
    connect(exit, SIGNAL(pressed()), this, SLOT(onExit()));

    properlyInitialized = true;

    ENSURE(this->checkProperlyInitialized(),  "RoadWindow.init() must end in properlyInitialized state");
}

void RoadWindow::setRoad(Road *road)
{
    fRoad = road;
}

void RoadWindow::onEditSpeedLimit()
{
    double val = askDouble();
    if (val != -1)
    {
        std::cout << "setter for speedlimits does not exist yet: " << val << std::endl;
    }
}