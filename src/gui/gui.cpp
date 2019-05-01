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
    this->setCentralWidget(fRoot);
    this->show();
    fRoot->setLayout(fLayout);

    QLabel* title = new QLabel("Project Software Engineering - BA1 Informatica - Thomas Dooms, Ward Gauderis, Mano Marichal - University of Antwerp");
    fLayout->addWidget(title, 0,0,1,3);

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

    fLayout-> addWidget(play, 1, 0 ,1, 1);
    fLayout-> addWidget(pause, 1, 1 ,1, 1);
    fLayout-> addWidget(skipOne, 1, 2 ,1, 1);

    connect(play, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(pause, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(skipOne, SIGNAL(pressed()), this, SLOT(onNext()));
}

std::string Window::askString(std::string example)
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling askString");

    bool ok;
    QString text = QInputDialog::getText(this, tr("enter your filename"),
                                         tr("filename"), QLineEdit::Normal,
                                         QString(example.c_str()), &ok);

    if (ok && !text.isEmpty())
        return text.toStdString();
    else return "";
}

double Window::askDouble(double min, double max, double step, double example)
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling askDouble");

    bool ok;
    double val = QInputDialog::getDouble(this, tr(""),
                                       tr("New value:"), example, min, max, step, &ok);
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

        fLayout->addWidget(temp, i+2, 0, 1, 3);
        fRoadButtons[temp] = roads[i];
    }
}

Window::state Window::getState() const
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling CreateButtons");

    state temp = fCrState;
    if(fCrState == next) fCrState = pause;
    return temp;
}

void Window::closeEvent (QCloseEvent *event)
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling CreateButtons");
    if (fCrState == busy) return;
    fCrState = quit;
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
    if (fCrState == busy) return;
    fCrState = play;
}

void Window::onPause()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onPause");
    if (fCrState == busy) return;
    fCrState = pause;
}

void Window::onNext()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onNext");
    if (fCrState == busy) return;
    fCrState = next;
}
void Window::onExit()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onExit");
    if (fCrState == busy) return;
    fCrState = quit;
}
void Window::onRoadButton()
{
    REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onRoadButton");

    if (fCrState == busy) return;
    fCrState = busy;

    QObject* obj = sender();

    RoadWindow window;
    window.setRoad(fRoadButtons[obj]);
    window.init();
    while (window.fCrState != quit) Window::delay(500);
    fCrState = inactive;
}

std::string Window::doubleToPrecision(double d, int precision)
{
    int x = 1;

    for (int k=0;k<precision;k++)
    {
        x *= 10;
    }

    std::string as_int = std::to_string(int(d * x + 0.5));
    std::string as_double = as_int.substr(0, as_int.size() - precision) + "," + as_int.substr(as_int.size() - precision, precision);

    return as_double;
}

//---------------------------------------ROAD WINDOW CLASS-----------------------------------------------------
void RoadWindow::init()
{
    REQUIRE(fRoad != NULL, "roadwindow has no road");

    fLastRow = 0;

    this->setWindowTitle(fRoad->getName().c_str());
    this->setCentralWidget(fRoot);
    this->show();

    fRoot->setLayout(fLayout);
    fRoot->show();

    QLabel* name = new QLabel(("Name: " + fRoad->getName()).c_str());
    fLayout->addWidget(name, 0,0,1,1);

    QLabel* length = new QLabel(("Length: " + std::to_string(int(fRoad->getRoadLength() + 0.5)) + "m").c_str());
    fLayout->addWidget(length, 1,0,1,1);

    std::string next;
    if (fRoad->getNextRoad() == NULL)
    {
        next = "this road has no next road";
    }
    else next = fRoad->getNextRoad()->getName();

    QLabel* nextinf = new QLabel(("Next Road: " + next).c_str());
    fLayout->addWidget(nextinf, 2,0,1,1);

    QFrame* hLine = new QFrame();
    hLine->setFrameShape(QFrame::HLine);

    fLayout->addWidget(hLine, 3, 0, 1, 2);

    fLastRow = 4;

    int row = updateTrafficLights(fLastRow);

    row++;
    QFrame* hLine2 = new QFrame();
    hLine2->setFrameShape(QFrame::HLine);
    fLayout->addWidget(hLine2, row, 0, 1, 2);

    fLastZoneRow = row;
    row = updateZones(row);

    QPushButton *exit = new QPushButton("Exit");
    fLayout->addWidget(exit, row+1, 0, 1, 2);
    connect(exit, SIGNAL(pressed()), this, SLOT(onExit()));

    properlyInitialized = true;

    ENSURE(this->checkProperlyInitialized(),  "RoadWindow.init() must end in properlyInitialized state");
}

void RoadWindow::setRoad(Road *road)
{
    fRoad = road;
}

void RoadWindow::onEditTLightColor()
{
    std::string val = askString("orange/red/green");

    QObject *obj = sender();

    if (val == "green" or val == "red" or val == "orange")
    {
        switch (val[0])
        {
            case 'g':
                fTrafficLights[obj]->setColor(TrafficLight::kGreen);
                break;
            case 'o':
                fTrafficLights[obj]->setColor(TrafficLight::kOrange);
                break;
            case 'r':
                fTrafficLights[obj]->setColor(TrafficLight::kRed);
                break;
            default:
                break;

        }

        updateTrafficLights(fLastRow);
    }

}

int RoadWindow::updateTrafficLights(int row)
{
    if (fRoad->getTrafficLights().size() == 0)
    {
        return row;
    }

    // TRAFFIC LIGHTS
    QLabel* tLights = new QLabel("Traffic lights:");
    row++;
    replaceInGrid(row, 0, tLights);


    for (unsigned int i=0;i<fRoad->getTrafficLights().size();i++)
    {
        QLabel* tLightPos = new QLabel(("Position: " + std::to_string(int(fRoad->getTrafficLights()[i]->getPosition() + 0.5))).c_str());
        row++;
        replaceInGrid(row, 0, tLightPos);

        std::string color;

        switch(fRoad->getTrafficLights()[i]->getColor())
        {
            case TrafficLight::kRed:
                color = "Red";
                break;
            case TrafficLight::kOrange:
                color = "Orange";
                break;
            case TrafficLight::kGreen:
                color = "Green";
                break;

        }

        QLabel* tLightColor = new QLabel(("Current color: " + color).c_str());
        row++;
        replaceInGrid(row, 0, tLightColor);

        QPushButton *editTLightColor = new QPushButton("Edit");
        connect(editTLightColor, SIGNAL(pressed()), this, SLOT(onEditTLightColor()));
        replaceInGrid(row, 1, editTLightColor);

        fTrafficLights[editTLightColor] = fRoad->getTrafficLights()[i];

        row++;
        fLayout->setRowStretch(row, 1);
    }
    return row;
}

int RoadWindow::updateZones(int row)
{
    QLabel* zones = new QLabel("Zones:");
    row++;
    replaceInGrid(row, 0, zones);

    for (unsigned int i=0;i<fRoad->getZones().size();i++)
    {
        double lenght;
        if (i != fRoad->getZones().size() - 1)
        {
            lenght = fRoad->getZones()[i+1]->getPosition() - fRoad->getZones()[i]->getPosition();
        }
        else lenght = fRoad->getRoadLength() - fRoad->getZones()[i]->getPosition();

        double prevLenght;
        if (i != 0)
        {
            prevLenght = fRoad->getZones()[i]->getPosition() - fRoad->getZones()[i-1]->getPosition();
        }
        else prevLenght = fRoad->getZones()[i]->getPosition();

        QLabel* zoneLength = new QLabel((std::to_string(int(prevLenght + 0.5)) + "m - " + std::to_string(int(lenght + 0.5)) + "m:").c_str());
        row++;
        replaceInGrid(row, 0, zoneLength);

        QLabel* zoneSpeed = new QLabel(("Speedlimit: " + doubleToPrecision(fRoad->getZones()[i]->getSpeedlimit(), 2) + " m/s").c_str());
        row++;
        replaceInGrid(row, 0, zoneSpeed);

        QPushButton *editSpeed = new QPushButton("Edit");
        connect(editSpeed, SIGNAL(pressed()), this, SLOT(onEditSpeedLimit()));
        replaceInGrid(row, 1, editSpeed);

        fZones[editSpeed] = fRoad->getZones()[i];

        row++;
        fLayout->setRowStretch(row, 1);
    }
    return row;
}

void RoadWindow::onEditSpeedLimit()
{
    double val = askDouble(0, 1000, 1, 60);
    if (val != -1)
    {
        QObject* obj = sender();
        fZones[obj]->setSpeedLimit(val);
        updateZones(fLastZoneRow);
    }
}

void RoadWindow::replaceInGrid(int row, int colum, QWidget* widget)
{
    QLayoutItem *item = fLayout->itemAtPosition(row, colum);

    if (item != NULL)
    {
        item->widget()->hide();
        fLayout->removeWidget(item->widget());
    }

    fLayout->addWidget(widget, row, colum, 1, 1);
}
void RoadWindow::createVehicleButtons() 
{
    REQUIRE(this->checkProperlyInitialized(), "RoadWindow was not properly initialized when calling createVehicleButtons");

}
