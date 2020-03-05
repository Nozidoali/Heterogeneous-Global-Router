#include "RMST.h"
#include <queue>
#include <functional>
#include <cassert>

using namespace std;

UnionFind :: UnionFind(int _size) {
    size = _size;
    parent = new int [size];
    for(int i=0;i<size;i++) {
        parent[i] = i;
    }
}

UnionFind :: ~UnionFind() {
    delete [] parent;
}

void UnionFind :: Merge(int x, int y) {
    parent[this->Find(x)] == parent[this->Find(y)];
}

int UnionFind :: Find(int x) {
    return (parent[x] == x) ? x : parent[x] = Find(parent[x]);
}

bool UnionFind :: isMerged(int x, int y) {
    return Find(x) == Find(y);
}

Edge :: Edge(Point _a, Point _b, int _ia, int _ib) {
    a = _a;
    b = _b;
    ia = _ia;
    ib = _ib;
}

RMST_Net_Solver :: RMST_Net_Solver(RoutingInst * _rst, Net * _net) {
    rst = _rst;
    net = _net;
}

RMST_Net_Solver :: ~RMST_Net_Solver() {
}

void * startRMST(RoutingInst *_rst, Net * _net) {
    RMST_Net_Solver * rmst = new RMST_Net_Solver(_rst, _net);
    return (void *) rmst;
}

void endRMST(void * rmst) {
    delete (RMST_Net_Solver *)rmst;
}

int solveSimple(void * rmst) {
    RoutingInst * rst = ((RMST_Net_Solver *)rmst)->rst;
    Net * net = ((RMST_Net_Solver *)rmst)->net;
    // return if do not need to connect
    if (net->numPins <= 1) {
        return SUCCESS;
    }
    net->nroute.numSegs = net->numPins-1;
    net->nroute.segments.reserve(net->nroute.numSegs);
    // for each pair of pins in the net
    for(int j=0;j<net->numPins-1;j++) {
        // create the L-shape segment
        net->nroute.segments.push_back(lSegment(rst, net->pins[j], net->pins[j+1]));
    }
    return SUCCESS;
}



int solveRMST(void * rmst) {
    RoutingInst * rst = ((RMST_Net_Solver *)rmst)->rst;
    Net * net = ((RMST_Net_Solver *)rmst)->net;
    // return if do not need to connect
    if (net->numPins <= 1) {
        return SUCCESS;
    }
    // Declare a priority queue to maintain the MST
    priority_queue<Edge> queue;

    // Add all the possible edges to the queue
    for(int i=0;i<net->numPins;i++) {
        for(int j=i+1;j<net->numPins;j++) {
            queue.push(Edge(net->pins[i], net->pins[j], i, j));
        }
    }

    int count = 0;
    // Prim's Algorithm to find the minimum spanning tree.

    // Reserve n-1 edge for n tree
    net->nroute.numSegs = net->numPins-1;
    net->nroute.segments.reserve(net->nroute.numSegs);

    // init Union Find Solver
    UnionFind uf(net->numPins);

    while(!queue.empty()) {
        Edge e = queue.top();
        queue.pop();
        if (uf.isMerged(e.ia,e.ib)) {
            continue;
        }
        else {
            uf.Merge(e.ia,e.ib);
            net->nroute.segments.push_back(lSegment(rst, e.a, e.b));
            count++;
        }
    }

    assert(count == net->numPins-1);
    return SUCCESS;
}