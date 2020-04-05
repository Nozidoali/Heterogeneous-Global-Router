#include "Logic_Astar.h"
#include "Task_Net.h"
#include <queue>
#include <iostream>
#include <map>
#include <algorithm>
#include <thread>


#include "Logic_UnionFind.h"
struct Edge {
    Point a,b;
    int ia,ib;
    bool operator < (const Edge e) const { return distance(a,b) > distance(e.a, e.b); };
    Edge(Point _a, Point _b, int _ia, int _ib) {
        a = _a; b = _b; ia = _ia; ib = _ib;
    }
};
NetTask :: NetTask( RoutingInst * _rst, Net * _net ) {
    rst = _rst;
    net = _net;

    // initialize overflow and wirelength
    overflow = wirelength = 0;

    // calculate boundarys
    x_lower = rst->gx; x_upper = 0;
    y_lower = rst->gy; y_upper = 0;
    for(int i=0;i<net->numPins;i++) {
        x_lower = min( x_lower, net->pins[i].x );
        x_upper = max( y_upper, net->pins[i].x );
        y_lower = min( y_lower, net->pins[i].y );
        y_upper = max( y_upper, net->pins[i].y );
    }
    assert( x_lower <= x_upper && y_lower <= y_upper );
    area = ( x_upper-x_lower ) * ( y_upper-y_upper );

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
            tasks.push_back( PinsTask( rst, net, a, b ) );
// =================================End=================================
        }
    }
}

NetTask :: ~NetTask() {
    tasks.clear();
}

void NetTask :: Solve() {
    
    // sort with respect to the estimated area.
    sort( tasks.begin(), tasks.end() );

    // clean the overflow and wirelength
    overflow = 0; wirelength = 0;

    // solve the pins tasks
    for ( auto& task : tasks ) {

        // continue if the task has been solved.
        if ( task.isDone() ) {
            continue;
        }
        
        // try 3 times to solve non-overflow solution
        task.Solve();
        int attempts = 3;
        while( attempts-- ) {
            if ( !task.isOverflow() ) { break; }
            task.TryHarder();
        }
    }

}


void NetTask :: Remove() {
    RemoveUtil();
    for(auto& task : tasks) {
        task.Clean();
    }
}

void NetTask :: RemoveUtil() {

    // create a new manager
    Astar_Man * pMan = new Astar_Man (
        rst,
        Point( x_lower, y_lower ),
        Point( x_upper, y_upper ),
        getMaxDifficulty()
    );

    // add solutions into manager
    for(auto& task : tasks) {
        pMan->AddSegment( task.getSolution() );
    }

    // update the utility of rst
    pMan->RemoveUtil();
    overflow = 0;
    wirelength = 0;

    delete pMan; pMan = NULL;

}

void NetTask :: Save() {

    // save the segments
    for(auto& task : tasks) {
        task.Save();
    }

}

void NetTask :: SaveUtil() {

    // create a new manager
    Astar_Man * pMan = new Astar_Man (
        rst,
        Point( x_lower, y_lower ),
        Point( x_upper, y_upper ),
        getMaxDifficulty()
    );

    // add solutions into manager
    for(auto& task : tasks) {
        pMan->AddSegment( task.getSolution() );
    }

    // update the utility of rst
    pMan->SaveUtil();
    overflow = pMan->overflow;
    wirelength = pMan->wirelength;

    delete pMan; pMan = NULL;

}

int NetTask :: getMaxDifficulty() {
    int ret = 0;
    for ( auto& task : tasks ) {
        ret = max( ret, task.getDifficulty() );
    }
    return ret;
}