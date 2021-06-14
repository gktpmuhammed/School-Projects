#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QTime>
#include <QWidget>
#include <QTimer>
#include "mytimer.h"
#include "gridlayout.h"
#include "wordbutton.h"
class MAinWindow : public QWidget { // Card subclass

    Q_OBJECT

public:
    MAinWindow(); // constructor
};

class MyGrid : public QWidget { // subclass of QWidget to implement grid layout for cards

    Q_OBJECT

private:
    QTimer *m_Wait; // timer for waiting after cards are shown

public slots:
    void newGame();

public:
    MyGrid(GridLayout *layout); // constructor
    bool player1; // boolean to detect if current player is player 1 or player2
};


#endif // MAINWINDOW_H
