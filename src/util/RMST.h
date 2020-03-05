#ifndef UNION_FIND_H
#define UNION_FIND_H

#include "../router.h"
#include <map>
class UnionFind {

public:
    int * parent;
    int size;
    UnionFind(int _size);
    ~UnionFind();
    void Merge(int x,int y);
    int Find(int x);
    bool isMerged(int x, int y);
};

struct Edge {
    Point a,b;
    int ia,ib;
    bool operator < (const Edge e) const { return distance(a,b) < distance(e.a, e.b); };
    Edge(Point _a, Point _b, int _ia, int _ib);
};
class RMST_Net_Solver {

public:
    RoutingInst * rst;
    Net * net;
    RMST_Net_Solver(RoutingInst *_rst, Net * _net);
    ~RMST_Net_Solver();
};

/**
 * void * CreateRMST(RoutingInst *rst, Net * net);
 * Return a new RMST solver to solve one net on the  RoutingInst rst.
 */
void * startRMST(RoutingInst *_rst, Net * _net);

/**
 * void endRMST(void * rmst);
 * Release the memory allocated.
 */
void endRMST(void * rmst);

/**
 * int solveRMST(void * rmst);
 * solve the network routing problem with Minimum Spanning Tree.
 */
int solveRMST(void * rmst);



#endif