//============================================================================
// @name        : 
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

void Window::init() {
    this->setWindowTitle("Simulation");
    this->setCentralWidget(root);
    this->show();
    root->setLayout(layout);

    properlyInitialized = true;
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


bool Window::checkProperlyInitialized()
{
    return properlyInitialized;
}

void Window::createButtons()
{

    QPushButton* play = new QPushButton("Play", this);
    QPushButton* pause = new QPushButton("Pause", this);
    QPushButton* skipOne = new QPushButton("Next tick", this);
    QPushButton* goBackOne = new QPushButton("Previous tick", this);

    play->show();
    pause->show();
    skipOne->show();
    goBackOne->show();

    int size = 70;

    play->setFixedHeight(size);
    pause->setFixedHeight(size);
    skipOne->setFixedHeight(size);
    goBackOne->setFixedHeight(size);

    layout-> addWidget(play, 1, 1 ,1, 1);
    layout-> addWidget(pause, 1, 2 ,1, 1);
    layout-> addWidget(skipOne, 1, 3 ,1, 1);
    layout-> addWidget(goBackOne, 1, 0 ,1, 1);
    layout->setRowStretch(0, this->size().height() - size);
    layout->setRowStretch(0, size);

    connect(play, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(pause, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(skipOne, SIGNAL(pressed()), this, SLOT(onNext()));
    connect(goBackOne, SIGNAL(pressed()), this, SLOT(onPrevious()));


    widgetsToDelete.emplace_back(play);
    widgetsToDelete.emplace_back(pause);
    widgetsToDelete.emplace_back(skipOne);
    widgetsToDelete.emplace_back(goBackOne);
}

void Window::onPlay()
{
    crState = play;
}

void Window::onPause() {
    crState = pause;
}

void Window::onNext() {
    crState = next;
}

void Window::onPrevious() {
    crState = previous;
}

Window::state Window::getState() const {
    return crState;
}


