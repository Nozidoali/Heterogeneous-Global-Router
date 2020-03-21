#include "Logic_Astar.h"

Astar_Man :: Astar_Man ( RoutingInst * rst) { 
    x_lower = 0; x_upper = rst->gx; 
    y_lower = 0; y_upper = rst->gy;
    area = (y_upper-y_lower)*(x_upper-x_lower);
    mem_distance = new int[area];
    mem_visited = new bool[area];
    mem_dir = new int[area];
    for(int i=0;i<area;i++) {
        mem_visited[i] = false;
        mem_distance[i] = -1;
        mem_dir[i] = -1; // -1 marks start point
    }
}

Astar_Man :: Astar_Man ( RoutingInst * rst, Point a, Point b , double Flex ) {
    int x_mean = ( min(a.x, b.x)+max(a.x, b.x) ) >> 1;
    int x_range = Flex*(( ( max(a.x, b.x)-min(a.x, b.x) ) >> 1 ) + 1);
    int y_mean = ( min(a.y, b.y)+max(a.y, b.y) ) >> 1;
    int y_range = Flex*(( ( max(a.y, b.y)-min(a.y, b.y) ) >> 1 ) + 1);
    assert( x_range > 0 );
    assert( y_range > 0 );
    x_lower = max( 0, x_mean-x_range ); 
    x_upper = min( rst->gx, x_mean+x_range+1 );            
    assert( x_lower < x_upper );
    y_lower = max( 0, y_mean-y_range ); 
    y_upper = min( rst->gy, y_mean+y_range+1 );
    assert( y_lower < y_upper );
    area = (y_upper-y_lower)*(x_upper-x_lower);
    mem_distance = new int[area];
    mem_visited = new bool[area];
    mem_dir = new int[area];
    for(int i=0;i<area;i++) {
        mem_visited[i] = false;
        mem_distance[i] = -1;
        mem_dir[i] = -1; // -1 marks start point
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
void Astar_Man :: retrace( RoutingInst * rst, Segment * segment, Point p ) {
    Point dir[4] = {Point(1,0),Point(-1,0),Point(0,1),Point(0,-1)};
    // find the previous point 
    Point point = p - dir[mem_dir[ToIndex(p)]];

    // add current fragment
    segment->AddFragment(point, p);
    rst->edgeUtils[rst->toIndex(point, p)]++;
    // end if we have reach the start point
    if ( point == segment->p1 ) {
        return;
    }
    // recursive
    retrace( rst, segment, point );


}
