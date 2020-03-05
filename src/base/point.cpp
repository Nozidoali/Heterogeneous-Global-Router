#include "point.h"

#include <sstream>
using namespace std;

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(int _x, int _y) {
    x = _x;
    y = _y;
}

const string Point::toString() {
    string str = "";
    str = "(" + to_string(x) + "," + to_string(y) + ")";
    return str;
}

bool Point :: operator == (const Point p) const {
    return x == p.x && y == p.y;
}
