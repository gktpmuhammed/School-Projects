#ifndef CMPE250_ASSIGNMENT2_RESTAURANTORGANIZER_H
#define CMPE250_ASSIGNMENT2_RESTAURANTORGANIZER_H
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class RestaurantOrganizer{

public:
    static const int MAX_TABLES = 100000;

    RestaurantOrganizer(const vector<int>& tableCapacityInput);
    void addNewGroup(int groupSize, ofstream& outFile);
    void heapUp(int index);
    void heapDown(int index);
    void printSorted(ofstream& outFile);

    unsigned int numberOfTables;
    int tableCapacity[MAX_TABLES];
    int tableCurrent[MAX_TABLES];
    int heap[MAX_TABLES];
};

#endif //CMPE250_ASSIGNMENT2_RESTAURANTORGANIZER_H
