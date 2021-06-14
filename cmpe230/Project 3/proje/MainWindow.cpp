#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QProcess>
#include "mytimer.h"
#include "gridlayout.h"
#include "wordbutton.h"
#include "mainwindow.h"


using namespace std;

int main(int argc, char *argv[]){

    QApplication app(argc, argv);  //  creates our application
    QWidget widget; //  creates widget that we add all of our buttons and labels
    QHBoxLayout *top, *bottom; // horizontal layout for time label, score label, quit and new game buttons
    QVBoxLayout *main; //  combination of top and bottom layouts
    int a=180; // maximum second that player can solve the game
    MyTimer *mt = new MyTimer(a); // Timer object for game time
    GridLayout *layout = new GridLayout(mt->timer,a); // creating layout
    MyGrid *game = new MyGrid(layout);  // creates our grid of cards
    QPushButton *new_game = new QPushButton("New game");  // creates new game button
    QObject::connect(new_game,SIGNAL(clicked()),game,SLOT(newGame()));
    QPushButton *quit_btn = new QPushButton("Quit");// creates quit game button
    QObject::connect(quit_btn,SIGNAL(clicked()),qApp,SLOT(quit()));
    QLabel *empty = new QLabel(""); //empty space between score label and new gam button
    QLabel *empty1 = new QLabel("");
    top = new QHBoxLayout;
    top->addWidget(mt->label);// adding time label to layout
    top->addWidget(layout->labels);// adding score label to layout
    top->addWidget(empty);
    top->addWidget(empty1);
    top->addWidget(new_game);//adding new game button to layout
    top->addWidget(quit_btn);// adding quit game button to layout


    bottom = new QHBoxLayout;  // bottom layout
    bottom->addWidget(game);   //adding wordbuttons to layout

    main = new QVBoxLayout;// main layout
    main->addLayout(top); //combining top and botttom layout
    main->addLayout(bottom);
    QSpacerItem *qs = new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Expanding);
    main->addSpacerItem(qs);//spacer ıtem that lifts grid to top
    widget.setLayout(main);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    widget.setWindowTitle("Card Match Game");//program name
    widget.resize(640,480);//resizeble window size
    widget.show();  // show widget



    return app.exec();
}

MyGrid::MyGrid(GridLayout *layout)  // constructor
{
    // creating grid layout
    setLayout(layout); // setting layout
    vector<QString> words;
    // setting words to the buttons randomly
    for(int i=0;i<2; i++){
        words.push_back("apple");
        words.push_back("orange");
        words.push_back("banana");
        words.push_back("lemon");
        words.push_back("melon");
        words.push_back("strawberry");
        words.push_back("mango");
        words.push_back("pineapple");
        words.push_back("coconut");
        words.push_back("apricot");
        words.push_back("peach");
        words.push_back("pomegranate");
        words.push_back("grape");
        words.push_back("cherry");
        words.push_back("pear");
    }
    srand((int)time(0));
    for(int row=1; row<6; row++){
        for(int col=0; col<6; col++){
            int choose = rand()%words.size();

            WordButton *randButton = new WordButton(words[choose],"?",(row-1)*6+col,layout);
            QObject::connect(randButton, SIGNAL(clicked()),
                             randButton, SLOT(show_card()));//for every click it shows the hidden word
            QObject::connect(randButton, SIGNAL(clicked()),
                             layout, SLOT(check_words()));//  for checking if two words are matched
            layout->addWidget(randButton, row, col, 1, 1);// adding buttons to layout
            words.erase(words.begin()+choose);//erasing already selected word from vector for random selection
        }
    }

}
//starts new game if new game button pressed
void MyGrid::newGame()  // function that starts new game
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments()); //application restart
}

