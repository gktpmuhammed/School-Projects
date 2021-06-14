#include "Table.h"

using namespace std;

Table::Table()
	: occupied{false}
{ }

void Table::occupy() {
    this->occupied = true;
}

void Table::empty() {
    this->occupied = false;
}

bool Table::isOccupied() const {
    return occupied;
}

ostream &operator<<(ostream &out, const Table &table) { 
    out << "Table is " << (table.isOccupied() ? "Occupied" : "Empty") << endl;
    return out;
}

