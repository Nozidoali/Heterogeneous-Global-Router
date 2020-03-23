#include "Logic_Astar.h"
#include <iostream>
using namespace std;

Astar_Man :: Astar_Man ( RoutingInst * _rst) { 
    rst = _rst;
    overflow = wirelength = 0;
    x_lower = 0; x_upper = rst->gx; 
    y_lower = 0; y_upper = rst->gy;
    area = (y_upper-y_lower)*(x_upper-x_lower);
    mem_distance = new int[area];
    mem_visited = new bool[area];
    mem_dir = new int[area];
    mem_map_x = new bool[area];
    mem_map_y = new bool[area];
    for(int i=0;i<area;i++) {
        mem_visited[i] = false;
        mem_distance[i] = -1;
        mem_dir[i] = -1; // -1 marks start point
        mem_map_x[i] = mem_map_y[i] = false;
   }
}

Astar_Man :: Astar_Man ( RoutingInst * _rst, Point a, Point b , double Flex ) {
    rst = _rst;
    overflow = wirelength = 0;
    start = a;
    end = b;
    double x_mean = ( min(a.x, b.x)+max(a.x, b.x) ) / 2.0;
    double x_range = (((double)( max(a.x, b.x)-min(a.x, b.x) ) / 2.0 ) + Flex);
    double y_mean = ( min(a.y, b.y)+max(a.y, b.y) ) / 2.0;
    double y_range = (((double)( max(a.y, b.y)-min(a.y, b.y) ) / 2.0 ) + Flex);
    assert( x_range >= 0.0 );
    assert( y_range >= 0.0 );
    x_lower = max( 0.0, x_mean-x_range ); 
    x_upper = min( (double)rst->gx, x_mean+x_range+1 );            
    assert( x_lower < x_upper );
    y_lower = max( 0.0, y_mean-y_range ); 
    y_upper = min( (double)rst->gy, y_mean+y_range+1 );
    assert( y_lower < y_upper );
    area = (y_upper-y_lower)*(x_upper-x_lower);
    mem_distance = new int[area];
    mem_visited = new bool[area];
    mem_dir = new int[area];
    mem_map_x = new bool[area];
    mem_map_y = new bool[area];
    for(int i=0;i<area;i++) {
        mem_visited[i] = false;
        mem_distance[i] = -1;
        mem_dir[i] = -1; // -1 marks start point
        mem_map_x[i] = mem_map_y[i] = false;
    }
}

/**
 * IsFull
 * 
 * return if the manager has the size of grid
 */
bool Astar_Man :: IsFull() {
    return x_upper==rst->gx && x_lower==0 && y_upper==rst->gy && y_lower==0;
}

/**
 * IsUsed
 * 
 * return if the edge between two points are recorded
 */
bool Astar_Man :: IsUsed( Point a, Point b ) {
    if(a.x==b.x) {
        Point ret(a.x, min( a.y, b.y ));
        return mem_map_y[ToIndex(ret)];
    }
    if(a.y==b.y) {
        Point ret(min( a.x, b.x ), a.y);
        return mem_map_x[ToIndex(ret)];
    }
}

/**
 * 
 * retrace
 * 
 * retrace the route from the start point to the end
 * note that this is implemented recursively
 * 
 */
void Astar_Man :: retrace( Point p ) {
    Point dir[4] = {Point(1,0),Point(-1,0),Point(0,1),Point(0,-1)};
    // find the previous point 
    Point point = p - dir[mem_dir[ToIndex(p)]];

    // add current fragment
    switch ( mem_dir[ToIndex(p)] )
    {
    case 0:
        mem_map_x[ToIndex(point)] = true;
        break;
    case 1:
        mem_map_x[ToIndex(p)] = true;
        break;
    case 2:
        mem_map_y[ToIndex(point)] = true;
        break;
    case 3:
        mem_map_y[ToIndex(p)] = true;
        break;
    default:
        break;
    }

    // end if we have reach the start point
    if ( point == start ) {
        return;
    }
    // recursive
    retrace( point );

}

/**
 * 
 * Update
 * 
 * Update the given segment with A star Manager
 * 
 */
void Astar_Man :: Update( Segment * segment ) {

//=========================Update Fragment==========================    
    // for each mem_x
    for(int y=y_lower;y<y_upper;y++)
    {
        int pos = x_lower;
        while (pos<x_upper) {
            if (mem_map_x[ToIndex(Point(pos,y))]) {
                Point start(pos, y);

                // find the end point of the fragment
                for(;pos<x_upper && mem_map_x[ToIndex(Point(pos,y))];pos++){}
                Point end(pos, y);

                // add the fragment and update 
                segment->AddFragment( start, end );

                // update overflow and wirelength
                wirelength++;
                overflow += rst->IsOverflow( start, end );

            }
            else {
                ++pos;
            }
            
        }
    }

    // for each mem_y
    for(int x=x_lower;x<x_upper;x++)
    {
        int pos = y_lower;
        while (pos<y_upper) {
            if (mem_map_y[ToIndex(Point(x,pos))]) {
                Point start(x, pos);

                // find the end point of the fragment
                for(;pos<y_upper && mem_map_y[ToIndex(Point(x,pos))];pos++){}
                Point end(x, pos);

                // add the fragment and update 
                segment->AddFragment( start, end );

                // update overflow and wirelength
                wirelength++;
                overflow += rst->IsOverflow( start, end );

            }
            else {
                ++pos;
            }
        }
    }

//=========================Update Util==============================    
    for(int y=y_lower;y<y_upper;y++) 
        for(int x=x_lower;x<x_upper;x++)
        {
            Point start(x,y);
            Point x_end = start + Point(1,0);
            Point y_end = start + Point(0,1);
            if (mem_map_x[ToIndex(Point(x,y))]) {
                ++rst->edgeUtils[rst->toIndex(start, x_end)];
            }
            if (mem_map_y[ToIndex(Point(x,y))]) {
                ++rst->edgeUtils[rst->toIndex(start, y_end)];
            }
        }

}
