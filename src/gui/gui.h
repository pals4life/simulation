//============================================================================
// @name        : gui.h
// @author      : Mano Marichal
// @date        :
// @version     :
// @copyright   : BA1 Informatica - Mano Marichal - University of Antwerp
// @description :
//============================================================================


#ifndef GOL_GUI_H
#define GOL_GUI_H

#include <QApplication>
#include <QRect>
#include <QPushButton>
#include <QPainter>
#include <QMainWindow>
#include <QInputDialog>
#include <QToolBar>
#include <QLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QCloseEvent>
#include <QObject>
#include <QLabel>
#include <QFrame>

#include <iostream>
#include <vector>
#include <cmath>
#include <map>

#include "../DesignByContract.h"
#include "../datatypes/Road.h"
#include "../datatypes/TrafficSigns.h"


class Window: public QMainWindow
{

    Q_OBJECT

public:
    enum EState {kInactive, kPlay, kPause, kNext, kQuit, kBusy, kPrint};
    explicit Window(QWidget* parent = nullptr);
    /**
     * ENSURE(this->properlyInitialized(), "Window.init() constructor must end in properlyInitialized state");
     */
    virtual void init();
    /**
     *
     * @return
     */
    bool checkProperlyInitialized() const;
    /**
     *
     * @param ms: the lenght of the delay in ms
     */
    static void delay(uint32_t ms);
    /**
     *
     */
    static void processEvents();
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling createButtons");
     */
    void createButtons();
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling getState");
     */
    EState getState() const;
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling closeEvent");
     */
    void closeEvent (QCloseEvent *event) override;
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling askString");
     */
    std::string askString(std::string example);
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling askDouble");
     */
    double askDouble(double min, double max, double step, double example);
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling createRoadButtons");
     */
    void createRoadButtons(const std::vector<Road*> &roads);
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling createRoadButtons");
     */
     void updateSimpleOutput(std::string output);

    static std::string doubleToPrecision(double d, int precision);

protected:

    bool properlyInitialized = false;
    mutable EState fCrState = kInactive;

    QWidget *fRoot = new QWidget(this);
    QGridLayout *fLayout = new QGridLayout;

private:
    std::map<QObject*, Road*> fRoadButtons;
    QLabel* fSimpleOutput;

private slots:
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onPlay");
     */
    void onPlay();
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onPause");
     */
    void onPause();
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onNext");
     */
    void onNext();
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onRoadButton");
     */
    void onRoadButton();
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onExit");
     */
    void onPrint();
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onExit");
     */
    void onExit();

public:
    /**
     * REQUIRE(this->checkProperlyInitialized(), "Window was not properly initialized when calling onExit");
     */
    void setCrState(EState fCrState);

};

class RoadWindow: public Window
{
    Q_OBJECT

public:
    /**
     * REQUIRE(fRoad != NULL, "roadwindow has no road");
     * ENSURE(this->checkProperlyInitialized(),  "RoadWindow.init() must end in properlyInitialized state");
     */
    void init() override;
    /**
     * Sets the road for the Roadwindow object
     * @param road: pointer to the road
     */
    void setRoad(Road* road);
    void createVehicleButtons();

private:
    int fLastRow;
    int fLastZoneRow;
    Road *fRoad = NULL;
    std::map<QObject*, const TrafficLight*> fTrafficLights;
    std::map<QObject*, const Zone*> fZones;

    int updateTrafficLights(int row);
    void replaceInGrid(int row, int colum, QWidget* widget);
    int updateZones(int row);

private slots:
    /**
     *
     */
    void onEditSpeedLimit();
    /**
     *
     */
    void onEditTLightColor();
};
#endif //GOL_GUI_H
