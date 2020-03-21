#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <map>
#include <algorithm>
#include <queue>
#include <functional>
#include <cassert>
#include "Solver.h"
#include "Logic_RoutingInst.h"
#include "Logic_SegTree.h"
#include "Logic_UnionFind.h"

/** 
 *  class of Hannan Grid which extrace the x and y coordinates of each pin
 *  Descrete the grid.
 */
class hGrid 
{
    
private:
    int * mem_x;
    int * mem_y;
    map<int, int> mem_ix;
    map<int, int> mem_iy;
    Point * pins;
public:
    int gx;
    int gy;
    hGrid(RoutingInst * rst, Net * net);
    ~hGrid();
    Point toIndex(Point p);
    Point toPoint(Point index);
};

struct Edge {
    Point a,b;
    int ia,ib;
    bool operator < (const Edge e) const { return distance(a,b) > distance(e.a, e.b); };
    Edge(Point _a, Point _b, int _ia, int _ib);
};

class RMST_Net_Solver
{
private:

public:
    RoutingInst * rst;
    Net * net;
    RMST_Net_Solver(RoutingInst *_rst, Net * _net);
    SegmentTree ** xTree, ** yTree;
    hGrid * hgrid;
    int Cost(Point a, Point b);
    void Connect(Point a, Point b);
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