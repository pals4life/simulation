//============================================================================
// @name        :
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

#include <iostream>
#include <vector>
#include <cmath>
#include <map>

#include "../DesignByContract.h"
#include "../datatypes/Road.h"


class Window: public QMainWindow
{

    Q_OBJECT

public:
    enum state {inactive, play, pause, next, previous, quit};
    /*
    * Constructor
    */
    explicit Window(QWidget* parent = nullptr);
    /*
     * init function
     */
    virtual void init();
    /*
    * Used to check if the grid is properly initialized and can be used
    */
    bool checkProperlyInitialized();
    /*
     * Used for delays in loops
     */
    static void delay(uint32_t ms);
    /*
     *
     */
    static void processEvents();
    /*
     * creates the UI-buttons
     */
    void createButtons();
    /*
     * Getters and Setters
     */
    state getState() const;
    /*
     * to close the window
     */
    void closeEvent (QCloseEvent *event) override;
    /*
     * ask for a string
     */
    std::string askString();
    double askDouble();
    /*
     * creates a pushbutton for every road
     */
    void createRoadButtons(const std::vector<Road*> &roads);

protected:

    bool properlyInitialized = false;
    mutable state crState = inactive;

    QWidget *root = new QWidget(this);
    QGridLayout *layout = new QGridLayout;

    std::vector<QWidget*> widgetsToDelete;

private:
    std::map<QObject*, Road*> fRoadButtons;

private slots:
    void onPlay();
    void onPause();
    void onNext();
    void onRoadButton();
    void onExit();
};


class RoadWindow: public Window
{
    Q_OBJECT

public:
    void init() override;
    void setRoad(Road* road);

private:
    Road *fRoad = NULL;

private slots:
    void onEditSpeedLimit();
};
#endif //GOL_GUI_H
