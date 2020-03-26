#include "Basic_Point.h"

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

bool Point :: operator == (const Point & p) const {
    return x == p.x && y == p.y;
}

bool Point :: operator < (const Point & p) const {
    return x < p.x && y < p.y;
}


int Point ::  operator ^ (const Point & p) const {
    return abs( this->x - p.x ) + abs( this->y - p.y );
}

Point Point :: operator + (const Point & p) {
    return Point( x+p.x, y+p.y );
}

Point Point :: operator - (const Point & p) {
    return Point( x-p.x, y-p.y );
}

Point UnitDirect( Point a, Point b ) {
    if (a.x == b.x)
        return Point( 0, a.y<b.y? 1: -1);
    else if (a.y == b.y)
        return Point( a.x<b.x? 1: -1, 0);
    else
        return Point(0,0);
}