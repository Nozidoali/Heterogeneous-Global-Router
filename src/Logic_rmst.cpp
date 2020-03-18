#include "Logic_rmst.h"
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
    parent[this->Find(x)] = parent[this->Find(y)];
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

SegmentTree :: SegmentTree(int _size) {
    size = _size;
    sum = new int [size<<2];
    lazy = new int [size<<2];
    for(int i=0;i<size<<1;i++) {
        sum[i] = 0;
        lazy[i] = 0;
    }
}


SegmentTree :: SegmentTree(int _size, int * val) {
    size = _size;
    sum = new int [size<<2];
    lazy = new int [size<<2];
    if (val != NULL) {
        build(1, size, ROOT, val);
        return;
    }
    for(int i=0;i<size;i++) {
        sum[i] = 0;
        lazy[i] = 0;
    }
}

SegmentTree :: ~SegmentTree() {
    delete [] sum;
    delete [] lazy;
}

int SegmentTree :: Sum(int left, int right) {
    if (left < right) {
        return query(left+1, right, 1, size, ROOT);
    }
    if (left > right) {
        return query(right+1, left, 1, size, ROOT);
    }
    else {
        return 0;
    }
}

void SegmentTree :: Add(int left, int right, int val) {
    if (left < right) {
        return update(left+1, right, 1, size, ROOT, val);
    }
    if (left > right) {
        return update(right+1, left, 1, size, ROOT, val);
    }
    else {
        return;
    }
}

int SegmentTree :: query(int left, int right, int l, int r, int index) {
    if (left <= l && r <= right) {
        return sum[index];
    }
    pushDown(index, r-l+1);
    int m = (l+r)>>1;
    int ret = 0;
    if (left<=m)
        ret += query(left, right, l, m, index<<1);
    if (m<right)
        ret += query(left, right, m+1, r, index<<1|1);
    return ret;
}

void SegmentTree :: pushUp(int index) {
    sum[index] = sum[index<<1] + sum[index<<1|1];
}

void SegmentTree :: pushDown(int index, int length) {
    if (lazy[index]!=0) {
        // if the relationship is addition 
        // lazy[index<<1] += lazy[index];
        // lazy[index<<1|1] += lazy[index];

        // if the relationship is modify
        lazy[index<<1] = lazy[index<<1|1] = lazy[index];

        // if the relationship is addition
        // sum[index<<1] += lazy[index]*(length-(length>>1));
        // sum[index<<1|1] += lazy[index]*(length>>1);
        
        // if the relationship is modify
        sum[index<<1] = lazy[index]*(length-(length>>1));
        sum[index<<1|1] = lazy[index]*(length>>1);
        
        lazy[index] = 0;
    }
}

void SegmentTree :: update(int left, int right, int l, int r, int index, int val) {
    if (left <= l && r <= right) {
        // if the relationship is relation
        // lazy[index] += val;
        // sum[index] += val*(r-l+1);

        // if the relationship is modification
        lazy[index] = val;
        sum[index] = val*(r-l+1);
        return;
    }
    pushDown(index, r-l+1);
    int m = (l+r)>>1;
    if (left <= m)
        update(left, right, l, m, index<<1, val);
    if (m < right)
        update(left, right, m+1, r, index<<1|1, val);
    pushUp(index);
}

void SegmentTree :: build(int left, int right, int index, int * val) {
    lazy[index] = 0;
    if (left==right) {
        sum[index] = val[index];
        return;
    }
    int m = (left+right)>>1;
    build(left,m,index<<1,val);
    build(m+1,right,index<<1|1,val);
    pushUp(index);
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