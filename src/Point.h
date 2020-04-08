#ifndef POINT_H
#define POINT_H

#include "Error.h"
#include <string>
#include <sstream>
#include <cassert>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <queue>
#include <list>
#include <algorithm>
#include <map>

using namespace std;

/**
 * A structure to represent a 2D Point. 
 */
struct Point 
{

    int x ; /* x coordinate ( >=0 in the routing grid)*/
    int y ; /* y coordinate ( >=0 in the routing grid)*/
    Point();
    Point( int _x, int _y );
    const string & toString();
    bool    operator == (const Point & p) const;
    bool    operator != (const Point & p) const;
    bool    operator <  (const Point & p) const;
    int     operator ^  (const Point & p) const;
    Point   operator *  (const Point & p) const;
    Point   operator +  (const Point & p) const;
    Point   operator += (const Point & p) const;
    Point   operator -= (const Point & p) const;
    bool    operator <= (const Point & p) const;
    bool    operator >= (const Point & p) const;
    Point   operator -  (const Point & p) const;
    Point   operator >> (const Point & p) const;

    pair<Point,Point> operator , (const Point & p) const;
};

ostream & operator << ( ostream & os, pair<Point, Point> edge );

static Point dir[4] = { Point(1,0),Point(-1,0),Point(0,1),Point(0,-1) };
enum DIRECT {
    RIGHT,
    LEFT,
    DOWN,
    UP,
    NONE
};

DIRECT toDirect( Point p );

#endif