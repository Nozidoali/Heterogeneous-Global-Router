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
    mem_dir = new DIRECT[area];
    mem_map_x = new bool[area];
    mem_map_y = new bool[area];
    for(int i=0;i<area;i++) {
        mem_visited[i] = false;
        mem_distance[i] = -1;
        mem_dir[i] = NONE; // NONE marks start point
        mem_map_x[i] = mem_map_y[i] = false;
   }
}

Astar_Man :: Astar_Man ( RoutingInst * _rst, Point a, Point b , double Flex ) {
    rst = _rst;
    overflow = wirelength = 0;
    start = a;
    end = b;
    assert( Flex >=0 );
    double x_mean = ( min(a.x, b.x)+max(a.x, b.x) ) / 2.0;
    double x_range = (((double)( max(a.x, b.x)-min(a.x, b.x) ) / 2.0 + Flex) * 1);
    double y_mean = ( min(a.y, b.y)+max(a.y, b.y) ) / 2.0;
    double y_range = (((double)( max(a.y, b.y)-min(a.y, b.y) ) / 2.0 + Flex) * 1);
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
    mem_dir = new DIRECT[area];
    mem_map_x = new bool[area];
    mem_map_y = new bool[area];
    for(int i=0;i<area;i++) {
        mem_visited[i] = false;
        mem_distance[i] = -1;
        mem_dir[i] = NONE; // -1 marks start point
        mem_map_x[i] = mem_map_y[i] = false;
    }
    assert( IsValid(start) );
    assert( IsValid(end) );
}

Astar_Man :: ~Astar_Man() {
    delete [] mem_dir;      mem_dir = NULL;
    delete [] mem_visited;  mem_visited = NULL;
    delete [] mem_distance; mem_distance = NULL;
    delete [] mem_map_x;    mem_map_x = NULL;
    delete [] mem_map_y;    mem_map_y = NULL;
}
bool Astar_Man :: IsValid( Point a ) {
    return a.x>=x_lower && a.x<x_upper && a.y>=y_lower && a.y<y_upper;
}
// Find the index coordinated with respect to the region
int Astar_Man :: ToIndex( Point a ) {
    return (a.x-x_lower) + (a.y-y_lower)*(x_upper-x_lower);
}
void Astar_Man :: visit( Point a ) {
    mem_visited[ToIndex(a)] = true;
}
bool Astar_Man :: isVisited( Point a ) {
    return mem_visited[ToIndex(a)];
}
bool Astar_Man :: go( Point a, DIRECT direct, int cost ) {
    bool ret = cost < mem_dir[ToIndex(a)] || mem_dir[ToIndex(a)] == NONE;
    if (ret) {
        mem_dir[ToIndex(a)] = direct;
        mem_distance[ToIndex(a)] = cost;
    }
    return ret;
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
    assert( false );
    return false;
}

/**
 * 
 * retrace
 * 
 * retrace the route from the start point to the end
 * note that this is implemented recursively
 * 
 * the record is stored in mem_map
 */
void Astar_Man :: retrace( Point p ) {
    // find the previous point 
    Point point = p - dir[mem_dir[ToIndex(p)]];

    // add current fragment
    switch ( mem_dir[ToIndex(p)] )
    {
    case RIGHT:
        mem_map_x[ToIndex(point)] = true;
        break;
    case LEFT:
        mem_map_x[ToIndex(p)] = true;
        break;
    case DOWN:
        mem_map_y[ToIndex(point)] = true;
        break;
    case UP:
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
 * @segment: the segment to store the fragments
 */
void Astar_Man :: Update( Segment * segment ) {
    
    overflow = wirelength = 0;
//=========================Update Fragment==========================    
    // for each mem_x
    for(int y=y_lower;y<y_upper;y++)
    {
        int pos = x_lower;
        while (pos<x_upper) {
            if (mem_map_x[ToIndex(Point(pos,y))]) {
                Point start(pos, y);
                // find the end point of the fragment
                for(;pos<x_upper && mem_map_x[ToIndex(Point(pos,y))];pos++) {
                    // update overflow and wirelength
                    wirelength++;
                    overflow+= rst->IsOverflow( Point(pos, y), Point(pos+1,y) );
                }
                Point end(pos, y);

                // add the fragment and update 
                segment->AddFragment( start, end );
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
                for(;pos<y_upper && mem_map_y[ToIndex(Point(x,pos))];pos++) {
                    // update overflow and wirelength
                    wirelength++;
                    overflow+= rst->IsOverflow( Point(x,pos), Point(x,pos+1) );
                }
                Point end(x, pos);

                // add the fragment and update 
                segment->AddFragment( start, end );
            }
            else {
                ++pos;
            }
        }
    }
}

void Astar_Man :: AddFragment( Point start, Point end ) {
    Point step = UnitDirect( start, end );
    DIRECT direct = toDirect(step);
#ifdef DEBUG
    cout << "pMan: +" << start.toString() << "-" << end.toString() << " : " << step.toString() << "-" << direct  << endl;
#endif
    for(Point p=start;p!=end;p=p+step) {
        switch (direct)
        {
        case RIGHT:
            mem_map_x[ToIndex(p)] = true;
            break;
        case LEFT:
            mem_map_x[ToIndex(p+step)] = true;
            break;
        case DOWN:
            mem_map_y[ToIndex(p)] = true;
            break;
        case UP:
            mem_map_y[ToIndex(p+step)] = true;
            break;
        
        default:
            break;
        }
    }
}

void Astar_Man :: AddSegment( Segment * segment ) {
    assert( segment ); 
    for( auto& frag : segment->fragments ) {
        AddFragment(frag.first, frag.second);
    }
}

void Astar_Man :: RemoveUtil() {
    assert( rst ); 

    overflow = wirelength = 0;

    for(int y=y_lower;y<y_upper;y++) 
        for(int x=x_lower;x<x_upper;x++)
        {
            Point start(x,y);
            Point x_end = start + Point(1,0);
            Point y_end = start + Point(0,1);
            if (mem_map_x[ToIndex(start)]) {
#ifdef DEBUG
                cout << "pMan: " << start.toString() << ">" << x_end.toString() << endl;
#endif
                --rst->edgeUtils[rst->toIndex(start, x_end)];
            }
            if (mem_map_y[ToIndex(start)]) {
#ifdef DEBUG
                cout << "pMan: " << start.toString() << "^" << y_end.toString() << endl;
#endif
                --rst->edgeUtils[rst->toIndex(start, y_end)];
            }
        }
}


void Astar_Man :: SaveUtil() {
    assert( rst ); 

    overflow = wirelength = 0;

    for(int y=y_lower;y<y_upper;y++) 
        for(int x=x_lower;x<x_upper;x++)
        {
            Point start(x,y);
            Point x_end = start + Point(1,0);
            Point y_end = start + Point(0,1);
            if (mem_map_x[ToIndex(start)]) {
                wirelength++;
                overflow += rst->IsOverflow(start, x_end);
#ifdef DEBUG
                cout << "pMan: " << start.toString() << ">" << x_end.toString() << endl;
#endif
                ++rst->edgeUtils[rst->toIndex(start, x_end)];
            }
            if (mem_map_y[ToIndex(start)]) {
                wirelength++;
                overflow += rst->IsOverflow(start, y_end);
#ifdef DEBUG
                cout << "pMan: " << start.toString() << "^" << y_end.toString() << endl;
#endif
                ++rst->edgeUtils[rst->toIndex(start, y_end)];
            }
        }
}
