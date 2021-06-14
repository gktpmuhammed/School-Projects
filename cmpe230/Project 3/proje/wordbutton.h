#ifndef WORDBUTTON_H
#define WORDBUTTON_H
#include <QPushButton>
#include "gridlayout.h"

class WordButton :public QPushButton
{
    Q_OBJECT
public:
    WordButton(const QString& word,
               const QString& text,int x,GridLayout *gl,
               QWidget* parent = 0);
    QString wordz;  //this is the word that button has.
    GridLayout *gl; //this is the grid layout of game.
    int x; //this is the index of this wordbutton on the grid layout.
    bool control=false; //this boolean controls if this button has matched.
public slots:
    void show_card();//this slot called when a button clicked.
};

#endif // WORDBUTTON_H
