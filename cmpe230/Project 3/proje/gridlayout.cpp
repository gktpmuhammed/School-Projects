#include "gridlayout.h"
#include "wordbutton.h"

GridLayout::GridLayout(QTimer *timer,int a): QGridLayout(){
    this->timer = timer;
    this->final=new QTimer();
    QObject::connect(this->final,SIGNAL(timeout()),this,SLOT(out()));
    this->a=a;
    this->final->start(1000*a);
    this->labels = new QLabel("Score: 0");
    this->timed=new QTimer();
}
void GridLayout::soru(){ //if two card is not matched, after 1 second this slot called and turn cards again.
    timed->stop();
    timed=new QTimer();
    WordButton *widget1 = qobject_cast<WordButton*>
            (this->itemAt(prevo)->widget());
    WordButton *widget2 = qobject_cast<WordButton*>
            (this->itemAt(now)->widget());
    widget1->setText("?");
    widget2->setText("?");
    prevo=-1;
    now=-1;
}
void GridLayout::out(){//if the time is over, this slot called and disable all the buttons.
    final->stop();
    for (int i = 0; i < this->count(); ++i)
    {
        WordButton *widget = qobject_cast<WordButton*>
                (this->itemAt(i)->widget());
        widget->setEnabled(false);
        if(widget->control==false){ //this statement close a button if the second button not choosed yet.
            widget->setText("?");
        }
    }
}
void GridLayout::check_words(){ //this slot called each wordbutton click.
    if(timed->isActive()){ //if there are two open cards, this timer block new card for 1 second.
        return;
    }
    bool all_same = true;
    if(now==-1){}
    else{  //this statement control if there is two card open.
        WordButton *widget1 = qobject_cast<WordButton*>
                (this->itemAt(prevo)->widget());
        WordButton *widget2 = qobject_cast<WordButton*>
                (this->itemAt(now)->widget());
        if(widget1->wordz==widget2->wordz){ //if two cards are same, buttons will be disabled and score will increase.
            widget1->setEnabled(false);
            widget2->setEnabled(false);
            widget1->control=true;
            widget2->control=true;
            score+=1;
            this->labels->setText("Score: " + QString::number(this->score));
            prevo=-1;
            now=-1;
        }
        else{ //if two cards are not same, they will be open for 1 second and closed again.
            timed->start(1000);
            QObject::connect(this->timed,SIGNAL(timeout()),this,SLOT(soru()));
            return;
        }
    }
    for (int i = 3; i < this->count(); ++i) //this loop controls if all the buttons are matched.(control will be true if matched.
    {
        WordButton *widget = qobject_cast<WordButton*>
                (this->itemAt(i)->widget());
        if(widget->control==false){
            all_same=false;
            break;
        }
    }

    if(all_same){ //if all buttons are matched, game will finished with messagebox.
        this->timer->stop();
        QMessageBox msgBox;
        msgBox.setText("You won!");
        msgBox.exec();
        msgBox.setStandardButtons(QMessageBox::Cancel);
        for (int i = 0; i < this->count(); ++i)
        {
            WordButton *widget = qobject_cast<WordButton*>
                    (this->itemAt(i)->widget());
            widget->setEnabled(false);
        }
    }
}
