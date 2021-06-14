#ifndef MYTIMER_H
#define MYTIMER_H
#include <QTimer>
#include <QLabel>
#include <QMessageBox>

class MyTimer : QObject
{
    Q_OBJECT
public:
    MyTimer(int a);
    int a;  //this is time of the game.
    QTimer *timer; //this timer counts seconds.
    QLabel *label; //this is label of time.
    int counter; //this integer counts seconds from starting.

public slots:
    void MyTimerSlot(); //this slot called every second.
};

#endif // MYTIMER_H
