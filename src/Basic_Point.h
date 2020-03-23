#ifndef POINT_H
#define POINT_H

#include <string>
#include <sstream>

using namespace std;

/**
 * A structure to represent a 2D Point. 
 */
class Point
{
public:
    int x ; /* x coordinate ( >=0 in the routing grid)*/
    int y ; /* y coordinate ( >=0 in the routing grid)*/
    Point();
    Point(int _x, int _y);
    const string toString();
    bool operator == (const Point p) const;
    bool operator < (const Point p) const;
    int operator ^ (const Point p) const;
    Point operator + (const Point p);
    Point operator - (const Point p);
};


#endif