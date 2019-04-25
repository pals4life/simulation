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


#include <random>
#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>



class Window: public QMainWindow
{

    Q_OBJECT

public:
    enum state {play, pause, next, previous, quit};
    /*
    * Constructor
    */
    explicit Window(QWidget* parent = nullptr);
    /*
     * init function
     */
    void init();
    /*
    * Used to check if the grid is properly initialized and can be used
    */
    bool checkProperlyInitialized();
    /*
     * Used for delays in loops
     */
    static void delay(uint32_t ms);
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
    void closeEvent (QCloseEvent *event);

private:

    bool properlyInitialized = false;
    state crState = pause;

    QWidget *root = new QWidget(this);
    QGridLayout *layout = new QGridLayout;

    std::vector<QWidget*> widgetsToDelete;

private slots:
    void onPlay();
    void onPause();
    void onNext();
    void onPrevious();

};



#endif //GOL_GUI_H
