#include <iostream>
#include <utility> 
#include <sstream>
#include "HelpStudents.h"
using namespace std;

void parseInput(ifstream& inFile,int &  Student,int &  N,int &  M, int & K,  vector < pair< pair <int,int> , int > > & ways);


int main(int argc, char* argv[]){
    // Command line arguments are malformed
    if (argc != 3) {
        // cerr should be used for errors
        cerr << "Run the code with the following command: ./cmpe250-assignment4 [input_file] [output_file]" << endl;
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

    int Student,N, M, K;
    vector < pair< pair <int,int> , int > > ways;
    parseInput(inFile, Student, N, M, K, ways);

    HelpStudents solver(N,M,K,ways);
    if(Student==1)
        outFile << solver.firstStudent() << endl;
    if(Student==2)
        outFile << solver.secondStudent() << endl;
    if(Student==3)
        outFile << solver.thirdStudent() << endl;
    if(Student==4)
        outFile << solver.fourthStudent() << endl;
    if(Student==5)
        outFile << solver.fifthStudent() << endl;

    outFile.close();
    return 0;
}


void parseInput(ifstream& inFile,int &  Student,int &  N,int &  M, int & K,  vector < pair< pair <int,int> , int > > & ways)
{
    string line, tmp;
    getline(inFile, line);
    istringstream linestream(line);
    getline(linestream, tmp, ' ');
    Student = stoi(tmp);
    getline(linestream, tmp, ' ');
    N = stoi(tmp);
    getline(linestream, tmp, ' ');
    M = stoi(tmp);
    getline(linestream, tmp, ' ');
    K = stoi(tmp);
    for (int i = 0; i < M; i++)
    {
    	int a,b,x;
        getline(inFile, line);
        istringstream linestream(line);
        getline(linestream, tmp, ' ');
        a = stoi(tmp);
        getline(linestream, tmp, ' ');
        b = stoi(tmp);
        getline(linestream, tmp, ' ');
        x = stoi(tmp);
     	ways.push_back(make_pair(make_pair(a,b),x));
    }

    inFile.close();
    cout << "Student,N,M,K:" << Student  << " " << N  << " " << M  << " " << K << endl;
    cout << "Edges:" << endl;
    for (int i = 0; i < ways.size(); i++){
        cout << ways[i].first.first << " " << ways[i].first.second << " " << ways[i].second << endl;
	}
}
