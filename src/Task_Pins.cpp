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
    difficulty  =   0;

    segment = NULL;
}

PinsTask :: ~PinsTask() {

}

static inline int Weight( int capacity, int util ) {
    return 1 + util + 10*(util>=capacity);
}

typedef pair<int, Point> DIS_POINT;

/**
 * 
 * Solve
 * 
 * Solve the shortest path problem within the current rst.
 * 1. store the result in PinsTask::segment
 * 2. store the overflow and wirelength
 * 
 * will NOT update the utility
 */
void PinsTask :: Solve() {

    // clean the previous result
    Clean();

    // clean up the overflow and wirelength
    overflow = 0;
    wirelength = 0;

    // find non-overflow route without update utility
    segment = Route( difficulty, true );

    // find overflow route without update utility
    if( segment == NULL )
        segment = Route( 0, false );
    
#ifdef DEBUG
    cout.width(10);
    cout << "-- pin " << start.toString() << "-" << end.toString();
    cout.width(10);
    cout << "OF: " << overflow;
    cout.width(10);
    cout << "WL: " << wirelength << endl;
#endif
    assert( segment != NULL );

}

/**
 * TryHarder()
 * 
 * increase the difficulty and try in a larger grid
 */
void PinsTask :: TryHarder() {
    difficulty += 5;
    Solve();
}

/**
 * Clean
 * 
 * clean the record of segment without cleanning the utility
 */
void PinsTask :: Clean() {

    // clean up overflow and wirelength
    overflow = wirelength = 0;

    if ( segment == NULL ) {
        return;
    }

    delete segment;
    segment = NULL;

}

/**
 * Remove
 * 
 * remove the capacity occupied by the stored segment 
 */
void PinsTask :: Remove() {
    
    // return if the task has not been solved
    if ( segment == NULL ) {
        return;
    }

    // remove the addition on the utility
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

#ifdef BEBUG
    cout << "\n=== Start Routing (" << (isOpt ? "opt" : "non-opt") << ") ===" << endl;
    cout << "parameter = " << parameter << endl;
#endif
    Segment * seg = new Segment( start, end );
    Astar_Man * pMan = new Astar_Man( rst, start, end, parameter );

    priority_queue<DIS_POINT, vector<DIS_POINT>, greater<DIS_POINT> > q;

    // set a to be the start point
    q.push( make_pair( distance( start, end ), start ) );
    pMan->visit( start );
    pMan->go( start, NONE, distance( start, end ) );

    // loop until finding end
    while( !q.empty() ) {
        DIS_POINT target = q.top();
#ifdef DEBUG
        cout << "\t@" << target.second.toString() << " : " << target.first << endl;
#endif
        pMan->visit( target.second );
        q.pop();
        // quit if reach the end point
        if (target.second == end) {
            pMan->retrace( end );
            pMan->Update( seg );
            
            // store the overflow and wirelength
            overflow = pMan->overflow;
            wirelength = pMan->wirelength;

            delete pMan; pMan = NULL;
            return seg;
        }
        for(unsigned i=0;i<4;i++) {
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
#ifdef DEBUG
            cout << "+" << point.toString() << " : " << cost;
            cout << " cap:" << rst->edgeCaps[index];
            cout << " util:" << rst->edgeUtils[index];
#endif
            if ( pMan->go( point, (DIRECT)i, cost ) ) {
#ifdef DEBUG
            cout << " --added. ";
#endif
                q.push( make_pair( cost, point ) );
            }
#ifdef DEBUG
            cout << endl;
#endif
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
 * save the fragments
 */
void PinsTask :: Save() {
    net->nroute.AddSegment( segment );
}

void PinsTask :: SaveUtil() {
    
    // return if the task has not been solved
    if ( segment == NULL ) {
        return;
    }

    // add utility
    for(auto& frag : segment->fragments) {
        for( Point p=frag.first; !(p==frag.second); p=p+UnitDirect(frag.first,frag.second) )
            rst->edgeUtils[rst->toIndex(p, p+UnitDirect(frag.first,frag.second))]++;        
    }
}