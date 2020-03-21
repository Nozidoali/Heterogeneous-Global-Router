#ifndef SOLVER_WEIGHTED_H
#define SOLVER_WEIGHTED_H

#include "Logic_RoutingInst.h"

#define WEIGHTED_SOLVER

static inline int Weight( int capacity, int util ) {
    int overflow = max( 0 , util-capacity );
    return 1 + overflow;
}

Segment * Link( RoutingInst * rst, Point a, Point b );
void SolveNet ( RoutingInst * rst, Net * net);

class Weighted_Solver
{
private:
    RoutingInst * rst;
public:
    Weighted_Solver(RoutingInst * _rst){ rst = _rst; }
    void Solve();
};

#endif