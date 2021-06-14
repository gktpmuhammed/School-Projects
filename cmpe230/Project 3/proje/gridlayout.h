#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H
#include <QGridLayout>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>

class GridLayout: public QGridLayout
{
    Q_OBJECT

public:
    GridLayout(QTimer* timer,int a);
    QTimer* timer; //this is the timer from mytimer.
    QTimer *timed; //this timer holds wrongly matched cards 1 second. It does not let other cards clicked.
    QTimer *final; //this timer sets disabled all the cards when the game finished and failed.
    int a;  //this integer holds time of the game.
    int prevo=-1;  //this integer holds previous card's index.
    int now=-1;  //this integer holds second cards index.
    int score=0;  //this integer holds score.
    QLabel *labels;  //this is score label.

public slots:
    void check_words(); //this slot called when a wordbutton clicked.
    void soru(); //this slot called when second wordbutton clicked and they are not same.
    void out();  //this slot called when final timer finished and game failed.
};

#endif // GRIDLAYOUT_H
