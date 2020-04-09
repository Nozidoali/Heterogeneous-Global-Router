#include "Point.h"

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(int _x, int _y) {
    x = _x;
    y = _y;
}

const string & Point::toString() {
    string str = "";
    str = "(" + to_string(x) + "," + to_string(y) + ")";
    return move(str);
}

bool Point :: operator == (const Point & p) const {
    return x == p.x && y == p.y;
}

bool Point :: operator != (const Point & p) const {
    return !(*this == p);
}

bool Point :: operator < (const Point & p) const {
    return x < p.x && y < p.y;
}

int Point ::  operator ^ (const Point & p) const {
    return abs( x - p.x ) + abs( y - p.y );
}

Point Point :: operator + (const Point & p) const {
    return Point( x+p.x, y+p.y );
}

Point Point :: operator - (const Point & p) const {
    return Point( x-p.x, y-p.y );
}

Point Point :: operator * (const Point & p) const {
    return Point( x, p.y );
}

int Point :: operator / (const Point & p) const {
    return abs( x-p.x ) * abs( y-p.y );
}


Point Point :: operator -= (const Point & p) const {
    return Point( min( x, p.x ), min( y, p.y ) );
}

Point Point :: operator += (const Point & p) const {
    return Point( max( x, p.x ), max( y, p.y ) );
}

bool Point :: operator <= (const Point & p) const {
    return x <= p.x && y <= p.y;
}
bool Point :: operator >= (const Point & p) const {
    return x >= p.x && y >= p.y;
}

Point Point :: operator >> ( const Point & p ) const {
    Point ret = *this;
    assert( x == p.x || y == p.y );
    if( x == p.x && y == p.y )
        return ret;

    if (x == p.x)
        ret.y += y<p.y? 1: -1;
    else
        ret.x += x<p.x? 1: -1;

    return ret;
}

pair<Point,Point> Point :: operator , (const Point & p) const {
    return make_pair( *this, p );
}

ostream & operator << ( ostream & os, pair<Point, Point> edge ) {
    os << edge.first.toString() << "-" << edge.second.toString();
    return os;
}

DIRECT toDirect( Point p ) { 
    return (DIRECT)( (p.x+p.y==-1) + (p.y==1)*2 ); 
}
