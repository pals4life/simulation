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

void Window::processEvents()
{
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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

    play->show();
    pause->show();
    skipOne->show();

    int size = 70;

    play->setFixedHeight(size);
    pause->setFixedHeight(size);
    skipOne->setFixedHeight(size);

    layout-> addWidget(play, 1, 1 ,1, 1);
    layout-> addWidget(pause, 1, 2 ,1, 1);
    layout-> addWidget(skipOne, 1, 3 ,1, 1);

    connect(play, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(pause, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(skipOne, SIGNAL(pressed()), this, SLOT(onNext()));


    widgetsToDelete.emplace_back(play);
    widgetsToDelete.emplace_back(pause);
    widgetsToDelete.emplace_back(skipOne);
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

Window::state Window::getState() const {
    return crState;
}

void Window::closeEvent (QCloseEvent *event)
{
    crState = quit;
}

std::string Window::askString() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("User name:"), QLineEdit::Normal,
                                         QString("test.txt"), &ok);
    if (ok && !text.isEmpty())
        return text.toStdString();
    else return "something went wrong";
}