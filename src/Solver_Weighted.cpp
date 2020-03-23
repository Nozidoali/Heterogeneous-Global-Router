#include "Solver_Weighted.h"
#include "Logic_Astar.h"
#include "Logic_UnionFind.h"
#include <queue>
#include <iostream>
#include <map>
using namespace std;

typedef pair<int, Point> DIS_POINT;


/**
 * Link
 * 
 * return the shortest path between two points
 */
Segment * Weighted_Solver :: Link( Point a, Point b, double _parameter ) {

    //
    double parameter = _parameter;

    Segment * segment = LinkOpt( a, b, 3 ); 

    if (segment != NULL)
        return segment;

    return LinkOnce( a, b, 0 );

}

/**
 *  LinkOnce
 * 
 *  Only Try One time to Link two points 
 * 
 *  @return segment: find the valid segment
 *  @return NULL: error occurs
 *  
 */
Segment * Weighted_Solver :: LinkOnce( Point a, Point b, double parameter ) {
    
// let segment->p1 to be the start point and p2 to be the end point
    Astar_Man * pMan = new Astar_Man( rst, a, b, parameter );
    Segment * segment = new Segment( a, b );
 
    priority_queue<DIS_POINT, vector<DIS_POINT>, greater<DIS_POINT> > q;
    Point dir[4] = {Point(1,0),Point(-1,0),Point(0,1),Point(0,-1)};

    // set a to be the start point
    q.push( make_pair( distance( a,b ), a ) );
    pMan->visit( a );
    pMan->go( a, -1, distance( a,b ) );
    
    // loop until finding b
    while( !q.empty() ) {
        DIS_POINT target = q.top();
        pMan->visit( target.second );
        q.pop();

        // quit if reach the end point
        if (target.second == b) {
            pMan->retrace( b );
            pMan->Update( segment );
            delete pMan;
            return segment;
        }
        for(unsigned int i=0;i<4;i++) {
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

// ==============================A* search=============================
            int cost = 0;
            // net cost of target
            cost += target.first - distance( target.second, b );
            // weight of the edge between target and point
            cost += Weight( rst->edgeCaps[index], rst->edgeUtils[index] );
            // weight of estimate distance
            cost += distance( point, b );

            if ( pMan->go( point, i, cost ) ) {
                q.push( make_pair( cost, point ) );
            }
        }
    }

    delete segment;
    delete pMan;
    return NULL;
}


/**
 *  LinkOpt
 * 
 *  Only Try One time to Link two points with NO Overflow
 * 
 *  @return segment: find the valid segment
 *  @return NULL: can not find that segment with no overflow
 *  
 */
Segment * Weighted_Solver :: LinkOpt( Point a, Point b, double parameter ) {

// let segment->p1 to be the start point and p2 to be the end point
    Astar_Man * pMan = new Astar_Man( rst, a, b, parameter );
    Segment * segment = new Segment( a, b );

    priority_queue<DIS_POINT, vector<DIS_POINT>, greater<DIS_POINT> > q;
    Point dir[4] = {Point(1,0),Point(-1,0),Point(0,1),Point(0,-1)};

    // set a to be the start point
    q.push( make_pair( distance( a,b ), a ) );
    pMan->visit( a );
    pMan->go( a, -1, distance( a,b ) );
    
    // loop until finding b
    while( !q.empty() ) {
        DIS_POINT target = q.top();
        pMan->visit( target.second );
        q.pop();

        // quit if reach the end point
        if (target.second == b) {
            pMan->retrace( b );
            pMan->Update( segment );
            delete pMan;
            return segment;
        }
        for(unsigned int i=0;i<4;i++) {
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
            if ( rst->edgeCaps[index] <= rst->edgeUtils[index] ) {
                continue;
            }
// ==============================A* search=============================
            int cost = 0;
            // net cost of target
            cost += target.first - distance( target.second, b );
            // weight of the edge between target and point
            cost += Weight( rst->edgeCaps[index], rst->edgeUtils[index] );
            // weight of estimate distance
            cost += distance( point, b );

            if ( pMan->go( point, i, cost ) ) {
                q.push( make_pair( cost, point ) );
            }
        }
    }

    delete segment;
    delete pMan;
    return NULL;
}

struct Edge {
    Point a,b;
    int ia,ib;
    bool operator < (const Edge e) const { return distance(a,b) > distance(e.a, e.b); };
    Edge(Point _a, Point _b, int _ia, int _ib);
};

void Weighted_Solver :: SolveNet ( Net * net) {
    
    // Declare a priority queue to maintain the MST
    priority_queue<Edge> queue;

    // Add all the possible edges to the queue
    for(int i=0;i<net->numPins;i++) {
        for(int j=i+1;j<net->numPins;j++) {
            queue.push(Edge(net->pins[i], net->pins[j], i, j));
        }
    }

    // init Union Find Solver
    UnionFind uf(net->numPins);

    int total = net->numPins;
    int count = 0;

    while(!queue.empty()) {
        Edge e = queue.top();
        queue.pop();
        if (uf.isMerged(e.ia,e.ib)) {
            // continue if we have already connected them
            continue;
        }
        else {

            // merge to indicate that we have already connected a and b
            uf.Merge(e.ia,e.ib);

            Point a = e.a;
            Point b = e.b;

// =================================Start=================================
            Segment * segment = Link( a, b, 1.0 );
            
            net->nroute.AddSegment( segment );
// =================================End=================================
        }
    }
}

void Weighted_Solver :: ReserveCap( double ratio ) {
    int area = rst->gx*rst->gy*2;
    for(int i=0;i<area;i++) {
        capacity[i] = rst->edgeCaps[i] * ratio;
        rst->edgeCaps[i] -= capacity[i];
    }
}

/**
 * ReleaseCap
 * 
 * release the capacity from the solver to the rst.
 * provide possibility to route the optimal solution
 * 
 * @return true: if the release might help
 * @return false: if we have tried the whole map
 * 
 */
bool Weighted_Solver :: ReleaseCap( Point a, Point b, double parameter ) {
    Astar_Man * pMan = new Astar_Man( rst, a, b, parameter );

    if ( pMan->IsFull() ) {
        // if we have try the whole map
        return false;
    }

    for(int x=pMan->x_lower;x<pMan->x_upper;x++)
        for(int y=pMan->y_lower;y<pMan->y_upper;y++)
        {
            Point start(x,y);
            Point dir[4] = {Point(1,0),Point(-1,0),Point(0,1),Point(0,-1)};
            for(int i=0;i<4;i++) {
                Point end = start + dir[i];
                int index = rst->toIndex( start, end );
                if (!pMan->IsValid(start) || !pMan->IsValid(end) ) {
                    // continue if not in the local network
                    continue;
                }
                if (capacity[index] <= 1) {
                    // continue if no capacity to release
                }

                // release the capacity
                --capacity[index];
                ++rst->edgeCaps[index];

            }

        }
    
    delete pMan;
    return true;
}

void Weighted_Solver :: Solve() {

    // start by reserve some of the capacity.   
    ReserveCap( 0 );

    // initial solution
    for(int i=0;i<rst->numNets;i++) {
        cerr << "Net: " << i+1 << " / " << rst->numNets << "\r";
        fflush(stderr);
        Net * net = rst->nets + i;
        SolveNet( net );
    }

}

Weighted_Solver :: Weighted_Solver( RoutingInst * _rst ) {
    rst = _rst;
    int area = rst->gx*rst->gy*2;
    capacity = new int [area];
    for(int i=0;i<area;i++) {
        capacity[i] = 0;
    }
}

Weighted_Solver :: ~Weighted_Solver() {
    delete [] capacity;
}