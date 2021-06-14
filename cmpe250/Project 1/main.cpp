#include <iostream>
#include <sstream>
#include <fstream>
#include "Restaurant.h"

using namespace std;

void parseInput(ifstream& infile, unsigned int& N, vector<string>& operations) {
    string line;

	// Read the first line, should contain N
	getline(infile, line);
    N = stoi(line);

	// Read the second line, then read parts delimited by commas
	getline(infile, line);

	istringstream linestream(line);

    string operation;
	while (getline(linestream, operation, ',')) {
		operations.push_back(operation);
	}

	infile.close();

	// Printing out what we have read, to double check
    cout << "Number of Tables (N): " << N << endl;
    cout << "List of Customer Operations: " << line << endl;
}

int main(int argc, char* argv[]) {

	// Command line arguments are malformed
    if (argc != 3) {
		// cerr should be used for errors
        cerr << "Run the code with the following command: ./cmpe250-assignment1 [input_file] [output_file]" << endl;
        return 1;
    }
    
	// Open the input and output files, check for failures
    ifstream infile(argv[1]);
	if (!infile) { // operator! is synonymous to .fail(), checking if there was a failure
		cerr << "There was a problem opening \"" << argv[1] << "\" as input file" << endl;
		return 1;
	}

    ofstream outfile(argv[2]);
	if (!outfile) {
		cerr << "There was a problem opening \"" << argv[2] << "\" as output file" << endl;
		return 1;
	}

	unsigned int N;
    vector<string> operations;
	parseInput(infile, N, operations);

	// Constructing the restaurant and executing operations on it
    Restaurant restaurant(N);
    restaurant.execute(operations);

	// Print the "restaurant" to output file
    outfile << restaurant;
    outfile.close();

    return 0;
}
