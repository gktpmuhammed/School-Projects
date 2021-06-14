#ifndef TABLE_H
#define TABLE_H

#include <iostream>

using namespace std;

class Table {
private:
	bool occupied;

public:
	Table();

	void occupy();
	void empty();
	bool isOccupied() const;

	friend ostream &operator<<(ostream &out, const Table &table);
};

#endif

