#ifndef BASIC_HGRID_H
#define BASIC_HGRID_H

#include <map>
#include <algorithm>
#include <cstdlib>
#include "Logic_RoutingInst.h"

/** 
 *  class of Hannan Grid which extrace the x and y coordinates of each pin
 *  Descrete the grid.
 */
class hGrid {
    
private:
    int * mem_x;
    int * mem_y;
    map<int, int> mem_ix;
    map<int, int> mem_iy;
    Point * pins;
public:
    int gx;
    int gy;
    hGrid(RoutingInst * rst, Net * net);
    ~hGrid();
    Point toIndex(Point p);
    Point toPoint(Point index);
};

#endif
