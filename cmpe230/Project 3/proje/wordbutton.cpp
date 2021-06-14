#include "wordbutton.h"

WordButton::WordButton(const QString& word,
                       const QString& text,int x,GridLayout *gl,
                       QWidget* parent)
    : QPushButton(text, parent)

{
    //setting parameters
    this->gl=gl;
    this->x=x;
    this->wordz=word;
}
void WordButton::show_card(){
    if(gl->timed->isActive()){// preventing third button click while two button is open
        return;
    }
    if(gl->prevo==-1){//if no button is open
        this->setText(wordz);// shows word that is hidden
        gl->prevo=x;// getting index of button
    }
    else if(gl->prevo==x){//if second click is on same button, do nothing

    }
    else{
        this->setText(wordz);//shows second buttons' word
        gl->now=x;
    }
}

