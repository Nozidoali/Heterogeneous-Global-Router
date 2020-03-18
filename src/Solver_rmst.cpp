#include "Solver_rmst.h"

hGrid::hGrid(RoutingInst * rst, Net * net) {
    // constructing hGrid
    mem_x = new int [net->numPins];
    mem_y = new int [net->numPins];
    for(int i=0;i<net->numPins;i++) {
        mem_x[i] = net->pins[i].x;
        mem_y[i] = net->pins[i].y;
    }
    // Clean the redundant by sort and unique
    sort(mem_x, mem_x+net->numPins);
    sort(mem_y, mem_y+net->numPins);
    gx = unique(mem_x, mem_x+net->numPins) - mem_x;
    gy = unique(mem_y, mem_y+net->numPins) - mem_y;

    // remember the index of each value
    for(int i=0;i<gx;i++) {
        mem_ix[mem_x[i]] = i;
    }
    for(int i=0;i<gy;i++) {
        mem_iy[mem_y[i]] = i;
    }
    // cout << "gx: " << gx << endl;
    // cout << "gy: " << gy << endl;
}

Point hGrid :: toIndex(Point p) {
    return Point(mem_ix[p.x], mem_iy[p.y]);
}
Point hGrid :: toPoint(Point p) {
    return Point(mem_x[p.x], mem_y[p.y]);
}

hGrid::~hGrid() {
    delete [] mem_x;
    delete [] mem_y;
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
    // initialize the hannan grid
    hgrid = new hGrid(rst, net);

    // initialize the segment tree (based on hannan grid)
    xTree = new SegmentTree * [hgrid->gy];
    yTree = new SegmentTree * [hgrid->gx];
    for(int i=0;i<hgrid->gy;i++) {
        xTree[i] = new SegmentTree (hgrid->gx);
    }
    for(int i=0;i<hgrid->gx;i++) {
        yTree[i] = new SegmentTree (hgrid->gy);
    }
}

RMST_Net_Solver :: ~RMST_Net_Solver() {
    delete hgrid;
    delete [] xTree;
    delete [] yTree;
}

int RMST_Net_Solver :: Cost(Point a, Point b) {
    Point ia = hgrid->toIndex(a);
    Point ib = hgrid->toIndex(b);
    // cout << "from " << ia.toString() << " to " << ib.toString() << endl;
    if (a.y != b.y) {
        return yTree[ia.x]->Sum(ia.y, ib.y);
    }
    if (a.x != b.x) {
        return xTree[ia.y]->Sum(ia.x, ib.x);
    }
    else {
        // should not occur
        return 0;
    }
}

void RMST_Net_Solver :: Connect(Point a, Point b) {
    Point ia = hgrid->toIndex(a);
    Point ib = hgrid->toIndex(b);
    net->nroute.segments.push_back(lSegment(rst, a, b));
    net->nroute.numSegs++;
    yTree[ia.x]->Add(ia.y, ib.y, -1);
    xTree[ia.y]->Add(ia.x, ib.x, -1);
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

int solveRMST(void * _rmst) {
    RMST_Net_Solver * rmst = (RMST_Net_Solver *)_rmst;
    Net * net = ((RMST_Net_Solver *)_rmst)->net;
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
    net->nroute.numSegs = 0;

    // init Union Find Solver
    UnionFind uf(net->numPins);
    // cout << net->id << endl;
    while(!queue.empty()) {
        Edge e = queue.top();
        queue.pop();
        if (uf.isMerged(e.ia,e.ib)) {
            // continue if we have already connected them
            continue;
        }
        else {
            // cout << "link: " << e.ia << " " << e.ib << endl;

            // merge to indicate that we have already connected a and b
            uf.Merge(e.ia,e.ib);

            // Two solutions of L-shape route
            Point mid1 = Point(e.a.x, e.b.y);
            Point mid2 = Point(e.b.x, e.a.y);

            // Check the cost of two solutions
            int cost1 = rmst->Cost(e.a, mid1) + rmst->Cost(mid1, e.b);
            // cout << "linked" << e.a.toString() << " to " << mid1.toString() << endl;
            // cout << "linked" << e.b.toString() << " to " << mid1.toString() << endl;
            int cost2 = rmst->Cost(e.a, mid2) + rmst->Cost(mid2, e.b);
            // cout << "linked" << e.a.toString() << " to " << mid2.toString() << endl;
            // cout << "linked" << e.b.toString() << " to " << mid2.toString() << endl;

            // with segment tree
            if (cost1 < cost2) {
                rmst->Connect(e.a, mid1);
                rmst->Connect(mid1, e.b);
            }
            else {
                rmst->Connect(e.a, mid2);
                rmst->Connect(mid2, e.b);
            }

            // without segment tree
            // net->nroute.segments.push_back(lSegment(rst, e.a, e.b));
            // net->nroute.numSegs++;

            count++;
        }
    }
    return SUCCESS;
}