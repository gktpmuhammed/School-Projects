/*
Student Name: 
Student Number: 
Project Number: 1
Compile Status: [SUCCESS/FAIL]
Running Status: [SUCCESS/FAIL]
Notes: Anything you want to say about your code that will be helpful in the grading process.
*/

#include "Restaurant.h"

using namespace std;

void Restaurant::execute(const vector<string>& operations) {
    for (const string& op : operations){
        cout << "Operation: " << op << "\t";
        if(op=="N"){
            for(int i = 1;i < tables.size()+1;i++){
                if(!tables.at(i-1).isOccupied()){
                    tables.at(i-1).occupy();
                    break;
                }
            }
        }
        else if(op=="S"){
            for(int k = tables.size();k> -1;k--){
                if(!tables.at(k-1).isOccupied()){
                    tables.at(k-1).occupy();
                    break;
                }
            }
        }
        else{
            tables.at(stoi(op)-1).empty();
        }
		/* YOU NEED TO IMPLEMENT THIS PART */

        cout << "State: " << *this << endl;
    }
}

Restaurant::Restaurant(unsigned int N)
	: tables{N}
{
    cout << "-Starting- \tState: " << *this << endl;
}

ostream &operator<<(ostream &out, Restaurant &restaurant) {
    for (Table table : restaurant.tables) {
        out << table.isOccupied();
    }
	return out;
}

