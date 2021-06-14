#include <iostream>
#include <sstream>
#include "RestaurantOrganizer.h"


using namespace std;

void parseInput(ifstream& inFile, vector<int>& tableCapacities, vector<int>& groupSizes);


int main(int argc, char* argv[]){

    // Command line arguments are malformed
    if (argc != 3) {
        // cerr should be used for errors
        cerr << "Run the code with the following command: ./cmpe250-assignment2 [input_file] [output_file]" << endl;
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

    vector<int> tableCapacities, groupSizes;
    parseInput(inFile, tableCapacities, groupSizes);

    //Restaurant Operations
    RestaurantOrganizer rest{tableCapacities};
    for(int i=0;i<groupSizes.size();i++){
        rest.addNewGroup(groupSizes[i], outFile);
    }
    rest.printSorted(outFile);

    outFile.close();
    return 0;
}


void parseInput(ifstream& inFile, vector<int>& tableCapacities, vector<int>& groupSizes) {

    string line;
    int n, q;
    // Read the first line, should contain N
    getline(inFile, line);
    n = stoi(line);

    // Read the second line, then read list of table capacities to a vector
    getline(inFile, line);

    istringstream linestream(line);

    string capacity;
    while (getline(linestream, capacity, ' ')) {
        tableCapacities.push_back(stoi(capacity));
    }

    // Read the third line, should contain Q,
    getline(inFile, line);
    q = stoi(line);

    // Read the fourth line, then read a list of group sizes as a vector
    getline(inFile, line);
    istringstream linestream2(line);

    string gr_size;
    while (getline(linestream2, gr_size, ' ')) {
        groupSizes.push_back(stoi(gr_size));
    }

    inFile.close();

    // Printing out what we have read, to double check

    cout << "Number of Tables (N): " << n << endl;
    cout << "Capacity of Tables: ";
    for (int i = 0; i < tableCapacities.size(); i++){
		if (i == 10) {
			cout << "... (and " << tableCapacities.size() - 10 << " more)";
			break;
		}
		cout << tableCapacities[i] << " ";
	}
    cout << endl << "Number of Incoming Groups (Q): " << q << endl;
    cout << "Size of the Groups: ";
    for (int i = 0; i < groupSizes.size(); i++){
		if (i == 10) {
			cout << "... (and " << groupSizes.size() - 10 << " more)";
			break;
		}
		cout << groupSizes[i] << " ";
	}
	cout << endl;
}
