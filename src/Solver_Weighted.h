#ifndef SOLVER_WEIGHTED_H
#define SOLVER_WEIGHTED_H

#include "Logic_RoutingInst.h"

#define WEIGHTED_SOLVER

static inline int Weight( int capacity, int util ) {
    return 1 + util;
}

class Weighted_Solver
{

private:
    RoutingInst * rst;
    int * capacity;
    Segment * Link ( Point a, Point b, double parameter );
    Segment * LinkOnce ( Point a, Point b, double parameter );
    Segment * LinkOpt ( Point a, Point b, double parameter );

    void SolveNet( Net * net );
    void ReserveCap( double ratio );
    bool ReleaseCap( Point a, Point b, double parameter );

public:
    Weighted_Solver( RoutingInst * _rst );
    ~Weighted_Solver();
    void Solve();
};

#endif