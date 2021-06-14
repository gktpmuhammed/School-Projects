/*
Student Name:
Student Number:
Project Number: 3
Compile Status: [SUCCESS/FAIL]
Running Status: [SUCCESS/FAIL]
Notes: Anything you want to say about your code that will be helpful in the grading process.
*/
#include "SeatOperations.h"

using namespace std;

SeatOperations::SeatOperations(int N, int M){
    // IMPLEMENT ME!
    this->M = M;
    this->N = N;
    lines[0] = vector<Person>(N);//it creates N person
    lines[1] = vector<Person>(M);//it creates M person

}

void SeatOperations::addNewPerson(int personType, const string& ticketInfo){
    // IMPLEMENT ME!
    char ticketLineType = ticketInfo.at(0);//it holds the line type whether it is A or B
    int lineNumber = stoi(ticketInfo.substr(1,ticketInfo.size()));// it holds the seat number
    //these are temporary variables for holding the prior sitting person
    int modulo, tmpLine, tmpSeatNumber, tmpType, tmpType3;
    char tmpLineType;
    if(ticketLineType == 'A'){
        modulo = (lineNumber-1) % N;//determines the place where the person will sit
        tmpLine = 0;
        tmpSeatNumber = lines[0][modulo].seatNumber;
        tmpType = lines[0][modulo].type;
        tmpLineType = lines[0][modulo].lineType;
        tmpType3 = lines[0][modulo].type3LastOperation;
        lines[0][modulo].seatNumber = lineNumber;
        lines[0][modulo].type = personType;
        lines[0][modulo].lineType = 'A';
        lines[0][modulo].line = 0;
        lines[0][modulo].type3LastOperation = 0;
        if(tmpType== 1){
            personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
        }
        else if(tmpType == 2){
            personTypeTwo(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
        else if(tmpType == 3){
            personTypeThree(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
    }
    else if(ticketLineType == 'B'){
        modulo =(lineNumber-1) % M;
        tmpLine = 1;
        tmpSeatNumber = lines[1][modulo].seatNumber;
        tmpType = lines[1][modulo].type;
        tmpLineType = lines[1][modulo].lineType;
        tmpType3 = lines[1][modulo].type3LastOperation;
        lines[1][modulo].seatNumber = lineNumber;
        lines[1][modulo].type = personType;
        lines[1][modulo].lineType = 'B';
        lines[1][modulo].line = 1;
        lines[1][modulo].type3LastOperation = 0;

        if(tmpType == 1){
            personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
        }
        else if(tmpType == 2){
            personTypeTwo(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
        else if(tmpType == 3){
            personTypeThree(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
    }
}

void SeatOperations::printAllSeats(ofstream& outFile){
    // IMPLEMENT ME!
    //prints the first line
    for(int i = 0; i < lines[0].size(); i++){
        if(lines[0][i].seatNumber!=0){
            outFile<<lines[0][i].type<<" "<<lines[0][i].lineType<<lines[0][i].seatNumber<<endl;
        }
        else{
            outFile<<"0"<<endl;
        }
    }
    //prints the second line
    for(int i = 0; i < lines[1].size(); i++){
        if(lines[1][i].seatNumber!=0){
            outFile<<lines[1][i].type<<" "<<lines[1][i].lineType<<lines[1][i].seatNumber<<endl;
        }
        else{
            outFile<<"0"<<endl;
        }
    }
}

void SeatOperations::personTypeOne(int lineNumber,int modulo, int personType, int line, char lineType, int type3){
    //these are again temporary variables for holding prior sitting person
    int  tmpLine, tmpSeatNumber, tmpType,tmpType3;
    char tmpLineType;
    //if person sits in first line
    if(line == 0){
        modulo = (lineNumber-1)%M;//it calculates modulo according to other lines' size
        tmpLine = lines[1][modulo].line;
        tmpSeatNumber = lines[1][modulo].seatNumber;
        tmpType = lines[1][modulo].type;
        tmpLineType = lines[1][modulo].lineType;
        tmpType3 = lines[1][modulo].type3LastOperation;
        lines[1][modulo].seatNumber = lineNumber;
        lines[1][modulo].type = personType;
        lines[1][modulo].line = 1;
        lines[1][modulo].lineType = lineType;
        lines[1][modulo].type3LastOperation = type3;
        if(tmpType == 1){
            personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
        }
        else if(tmpType == 2){
            personTypeTwo(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
        else if(tmpType == 3){
            personTypeThree(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }

    }
    //if person sits in second line
    else if(line == 1){
        modulo = (lineNumber-1)%N;
        tmpLine = lines[0][modulo].line;
        tmpSeatNumber = lines[0][modulo].seatNumber;
        tmpType = lines[0][modulo].type;
        tmpLineType = lines[0][modulo].lineType;
        tmpType3 = lines[0][modulo].type3LastOperation;
        lines[0][modulo].seatNumber = lineNumber;
        lines[0][modulo].type = personType;
        lines[0][modulo].line = 0;
        lines[0][modulo].lineType = lineType;
        lines[0][modulo].type3LastOperation = type3;
        if(tmpType == 1){
            personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
        }
        else if(tmpType == 2){
            personTypeTwo(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
        else if(tmpType == 3){
            personTypeThree(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
    }
}

void SeatOperations::personTypeTwo(int modulo, int personType,int line, char lineType, int seatNumber, int type3){

    int  tmpLine, tmpSeatNumber, tmpType, tmpType3;
    char tmpLineType;
    if(line == 0){
        //this if controls whether the person sits in the last index of the first line
        if(modulo == N-1){
            //if person sits in the last index then these statements sends this person to other lines' first index
            tmpLine = lines[1][0].line;
            tmpSeatNumber = lines[1][0].seatNumber;
            tmpType = lines[1][0].type;
            tmpLineType = lines[1][0].lineType;
            tmpType3 = lines[1][0].type3LastOperation;
            lines[1][0].type = personType;
            lines[1][0].seatNumber = seatNumber;
            lines[1][0].line = 1;
            lines[1][0].lineType = lineType;
            lines[1][0].type3LastOperation = type3;
            if(tmpType == 1){
                personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
            }
            else if(tmpType == 2){
                personTypeTwo(0,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
            }
            else if(tmpType == 3){
                personTypeThree(0,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
            }
        }
        else{
            //if person sits in index other than last index the these statements controls successor index
            tmpLine = lines[0][modulo+1].line;
            tmpSeatNumber = lines[0][modulo+1].seatNumber;
            tmpType = lines[0][modulo+1].type;
            tmpLineType = lines[0][modulo+1].lineType;
            tmpType3 = lines[0][modulo+1].type3LastOperation;
            lines[0][modulo+1].type = personType;
            lines[0][modulo+1].seatNumber = seatNumber;
            lines[0][modulo+1].line = 0;
            lines[0][modulo+1].lineType = lineType;
            lines[0][modulo+1].type3LastOperation = type3;
            if(tmpType == 1){
                personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
            }
            else if(tmpType == 2){
                personTypeTwo(modulo+1,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
            }
            else if(tmpType == 3){
                personTypeThree(modulo+1,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
            }
        }

    }
    else if(line == 1){
        //this if controls whether the person sits in the last index of the first line
        if(modulo == M-1){
            //if person sits in the last index then these statements sends this person to other lines' first index
            tmpLine = lines[0][0].line;
            tmpSeatNumber = lines[0][0].seatNumber;
            tmpType = lines[0][0].type;
            tmpLineType = lines[0][0].lineType;
            tmpType3 = lines[0][0].type3LastOperation;
            lines[0][0].type = personType;
            lines[0][0].seatNumber = seatNumber;
            lines[0][0].line = 0;
            lines[0][0].lineType = lineType;
            lines[0][0].type3LastOperation = type3;
            if(tmpType == 1){
                personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
            }
            else if(tmpType == 2){
                personTypeTwo(0,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
            }
            else if(tmpType == 3){
                personTypeThree(0,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
            }
        }
        else{
            //if person sits in index other than last index the these statements controls successor index
            tmpLine = lines[1][modulo+1].line;
            tmpSeatNumber = lines[1][modulo+1].seatNumber;
            tmpType = lines[1][modulo+1].type;
            tmpLineType = lines[1][modulo+1].lineType;
            tmpType3 = lines[1][modulo+1].type3LastOperation;
            lines[1][modulo+1].type = personType;
            lines[1][modulo+1].seatNumber = seatNumber;
            lines[1][modulo+1].line = 1;
            lines[1][modulo+1].lineType = lineType;
            lines[1][modulo+1].type3LastOperation = type3;
            if(tmpType == 1){
                personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
            }
            else if(tmpType == 2){
                personTypeTwo(modulo+1,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
            }
            else if(tmpType == 3){
                personTypeThree(modulo+1,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
            }
        }
    }
}

void SeatOperations::personTypeThree(int modulo, int personType,int line, char lineType, int seatNumber, int type3){
    int tmpLine, tmpSeatNumber, tmpType,tmpType3;
    char tmpLineType;
    if(line == 0){
        type3 += 1;
        modulo += (2*(type3-1)+1);//modulo represents seatnumber and this statement updates seat number by lastoperation

        modulo %= (M+N);//takes the modulo of seatnumber
        //if seatnumber is bigger than N then this states that person needs to sit in other line
        if(modulo >= N){
            modulo -= N;//it subtract N from seatnumber to determine the place where person will sit
            tmpLine = 1;
            tmpSeatNumber = lines[1][modulo].seatNumber;
            tmpType = lines[1][modulo].type;
            tmpLineType = lines[1][modulo].lineType;
            tmpType3 = lines[1][modulo].type3LastOperation;
            lines[1][modulo].seatNumber = seatNumber;
            lines[1][modulo].type = personType;
            lines[1][modulo].lineType = lineType;
            lines[1][modulo].line = 1;
            lines[1][modulo].type3LastOperation = type3;
        }
        else{
            tmpLine = 0;
            tmpSeatNumber = lines[0][modulo].seatNumber;
            tmpType = lines[0][modulo].type;
            tmpLineType = lines[0][modulo].lineType;
            tmpType3 = lines[0][modulo].type3LastOperation;
            lines[0][modulo].seatNumber = seatNumber;
            lines[0][modulo].type = personType;
            lines[0][modulo].lineType = lineType;
            lines[0][modulo].line = 0;
            lines[0][modulo].type3LastOperation = type3;
        }

        if(tmpType == 1){
            personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
        }
        else if(tmpType == 2){
            personTypeTwo(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
        else if(tmpType == 3){
            personTypeThree(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }

    }
    else if(line == 1){
        type3 +=1;
        modulo += (2*(type3-1)+1);
        modulo += N;
        modulo %= (M+N);
        if(modulo >= N){
            modulo -=N;
            tmpLine = lines[1][modulo].line;
            tmpSeatNumber = lines[1][modulo].seatNumber;
            tmpType = lines[1][modulo].type;
            tmpLineType = lines[1][modulo].lineType;
            tmpType3 = lines[1][modulo].type3LastOperation;
            lines[1][modulo].seatNumber = seatNumber;
            lines[1][modulo].type = personType;
            lines[1][modulo].lineType = lineType;
            lines[1][modulo].line = 1;
            lines[1][modulo].type3LastOperation = type3;
        }
        else{
            tmpLine = lines[0][modulo].line;
            tmpSeatNumber = lines[0][modulo].seatNumber;
            tmpType = lines[0][modulo].type;
            tmpLineType = lines[0][modulo].lineType;
            tmpType3 = lines[0][modulo].type3LastOperation;
            lines[0][modulo].seatNumber = seatNumber;
            lines[0][modulo].type = personType;
            lines[0][modulo].lineType = lineType;
            lines[0][modulo].line = 0;
            lines[0][modulo].type3LastOperation = type3;
        }

        if(tmpType == 1){
            personTypeOne(tmpSeatNumber,modulo,tmpType,tmpLine,tmpLineType,tmpType3);
        }
        else if(tmpType == 2){
            personTypeTwo(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
        else if(tmpType == 3){
            personTypeThree(modulo,tmpType,tmpLine,tmpLineType,tmpSeatNumber,tmpType3);
        }
    }
}


// YOU CAN ADD YOUR HELPER FUNCTIONS