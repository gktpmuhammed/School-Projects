/*
Student Name:
Student Number:
Project Number: 5
Compile Status: [SUCCESS/FAIL]
Running Status: [SUCCESS/FAIL]
Notes: Anything you want to say about your code that will be helpful in the grading process.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]){
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);
    string line, tmp;
    getline(inFile, line);
    istringstream linestream(line);
    getline(linestream, tmp, ' ');
    long long int N = stoi(tmp);
    getline(linestream, tmp, ' ');
    long long int M = stoi(tmp);
    long long int originalArr[N];
    for(int i = 0; i < N; i++){
        inFile >> originalArr[i];
    }

    sort(originalArr,originalArr+N);

    long long int pairArr[N];//sum of consecutive N numbers
    long long int sum = 0;
    long long int sum2 = 0;
    long long int sumArr[N];//sum of all numbers to the N
    for(int k = 0; k < N; k++){
        //first M pairs are the sum of numbers to the M
        if(k < M){
            sum += originalArr[k];
            pairArr[k] = sum;
        }
        //others are addition of new number and subtraction of first number of old pair
        else{
            pairArr[k] = pairArr[k-1] + originalArr[k];
            pairArr[k] = pairArr[k] - originalArr[k-M];
        }
        sum2 += originalArr[k];
        sumArr[k] = sum2;
    }

    long long int result[N];
    //results are equal to itself
    for(int y = 0; y < M; y++){
        result[y] = pairArr[y];
        outFile << result[y] << " ";
    }
    //it is like recursive addition such as Result[4] = result[2] + sumArr[2]
    for(int j = M; j < N; j++){
        result[j] = result[j-M] + sumArr[j];
        outFile << result[j] << " ";
    }

    return 0;
}
