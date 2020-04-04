#include "Task_Pins.h"

PinsTask :: PinsTask( RoutingInst * _rst, Net * _net, Point _start, Point _end ) {

// assignments
    rst     =   _rst;
    net     =   _net;
    start   =   _start;
    end     =   _end;

// initialization
    overflow    =   0;
    wirelength  =   0;

    segment = NULL;
}

PinsTask :: ~PinsTask() {

}

static inline int Weight( int capacity, int util ) {
    return 1 + util;
}

typedef pair<int, Point> DIS_POINT;

void PinsTask :: Solve( double parameter ) {

    segment = Route( parameter, true );

    if( segment == NULL )
        segment = Route( 1, false );
    
    assert( segment );

}

void PinsTask :: Remove() {
    for(auto& frag : segment->fragments) {
        for( Point p=frag.first; !(p==frag.second); p=p+UnitDirect(frag.first,frag.second) )
            rst->edgeUtils[rst->toIndex(p, p+UnitDirect(frag.first,frag.second))]--;        
    }
    delete segment;
    segment = NULL;
}


/**
 * 
 * Route
 * 
 * the interface of the task class
 * 
 */
Segment * PinsTask :: Route( double parameter, bool isOpt ) {

    Segment * seg = new Segment( start, end );
    Astar_Man * pMan = new Astar_Man( rst, start, end, parameter );

    priority_queue<DIS_POINT, vector<DIS_POINT>, greater<DIS_POINT> > q;
    Point dir[4] = {Point(1,0),Point(-1,0),Point(0,1),Point(0,-1)};

    // set a to be the start point
    q.push( make_pair( distance( start, end ), start ) );
    pMan->visit( start );
    pMan->go( start, -1, distance( start, end ) );

    // loop until finding end
    while( !q.empty() ) {
        DIS_POINT target = q.top();
        pMan->visit( target.second );
        q.pop();
        // quit if reach the end point
        if (target.second == end) {
            pMan->retrace( end );
            pMan->Update( seg );
            overflow = pMan->overflow;
            wirelength = pMan->wirelength;
            delete pMan; pMan = NULL;
            return seg;
        }
        for(int i=0;i<4;i++) {
            Point point = target.second + dir[i];
            int index = rst->toIndex( point, target.second );

            // continue if point is out of range
            if ( !pMan->IsValid( point ) ) {
                continue;
            }
            // continue if point is visited
            if ( pMan->isVisited( point ) ) {
                continue;
            }
            // continue if edge is out of capacity
            if ( isOpt && rst->edgeCaps[index] <= rst->edgeUtils[index] ) {
                continue;
            }

// ==============================A* search=============================
            int cost = 0;
            // net cost of target
            cost += target.first - distance( target.second, end );
            // weight of the edge between target and point
            cost += Weight( rst->edgeCaps[index], rst->edgeUtils[index] );
            // weight of estimate distance
            cost += distance( point, end );

            if ( pMan->go( point, i, cost ) ) {
                q.push( make_pair( cost, point ) );
            }
        }
    }
    delete seg;     seg = NULL;
    delete pMan;    pMan = NULL;
    return NULL;
}

/**
 * 
 * Save
 * 
 * save the solution to the net
 * 
 */
void PinsTask :: Save() {
    net->nroute.AddSegment( segment );
}