#include "mytimer.h"

MyTimer::MyTimer(int a) : QObject()
{
    this->counter=0;
    this->a=a;
    this->label = new QLabel("Time(secs): 0");
    this->timer = new QTimer();
    QObject::connect(this->timer,SIGNAL(timeout()),this,SLOT(MyTimerSlot()));

    this->timer->start(1000);
}


void MyTimer::MyTimerSlot(){ //this slot called every second.
    this->counter+=1;
    this->label->setText("Time(secs): " + QString::number(this->counter)); //this label shows elapsed time.
    if(this->counter >= a){  //if counter reach time constraint, game will finished with fail messagebox.
        this->timer->stop();
        QMessageBox *qm = new QMessageBox();
        qm->setText("You Failed!");
        qm->setStandardButtons(QMessageBox::Cancel);
        qm->exec();
    }
}
