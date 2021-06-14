#include <iostream>
#include <utility> 
#include <sstream>
#include "SeatOperations.h"

using namespace std;

void parseInput(ifstream& inFile,int &N,int &M,int &Q, vector < pair <int,string> > & queries);


int main(int argc, char* argv[]){

    // Command line arguments are malformed
    if (argc != 3) {
        // cerr should be used for errors
        cerr << "Run the code with the following command: ./cmpe250-assignment3 [input_file] [output_file]" << endl;
        return 1;
    }

    // Open the input and output files, check for failures
    ifstream inFile(argv[1]);
    if (!inFile) { // operator! is synonymous to .fail(), checking if there was a failure
        cerr << "There was a problem opening \"" << argv[1] << "\" as input file" << endl;
        return 1;
    }

    ofstream outFile(argv[2]);
    if (!outFile) {
        cerr << "There was a problem opening \"" << argv[2] << "\" as output file" << endl;
        return 1;
    }

    int N, M, Q;
    vector < pair <int,string> > queries;
    parseInput(inFile, N, M, Q, queries);

    //Seat Operations
    SeatOperations plane(N, M);
    for (int i = 0; i < Q; i++){
        plane.addNewPerson(queries[i].first, queries[i].second);
    }
    plane.printAllSeats(outFile);

    outFile.close();
	return 0;
}


void parseInput(ifstream& inFile, int &N, int &M, int &Q, vector < pair <int,string> >& queries) {

    string line, tmp;
    getline(inFile, line);
    istringstream linestream(line);
    getline(linestream, tmp, ' ');
    N = stoi(tmp);
    getline(linestream, tmp, ' ');
    M = stoi(tmp);
    getline(linestream, tmp, ' ');
    Q = stoi(tmp);
    for (int i = 0; i < Q; i++)
    {
        int personType;
        string seat;
        getline(inFile, line);
        istringstream linestream(line);
        getline(linestream, tmp, ' ');
        personType = stoi(tmp);
        getline(linestream, seat, ' ');
        queries.push_back(make_pair(personType, seat));
    }

    inFile.close();

    // Printing out what we have read, to double check
    cout << "Size of the Line A (N): " << N << endl;
    cout << "Size of the Line B (M): " << M << endl;
    cout << "Queries (Q):" << endl;
    for (int i = 0; i < queries.size(); i++){
        cout << queries[i].first << " " << queries[i].second << endl;
	}
	cout << endl;
}