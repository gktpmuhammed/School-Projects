#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <iostream>
#include <vector>
#include "Table.h"

using namespace std;

class Restaurant {
private:
	vector<Table> tables;

public:
	Restaurant(unsigned int N);

	void execute(const vector<string>& operations);

	friend ostream &operator<<(ostream &out, Restaurant &restaurant);
};

#endif

