#include "solver.h"

#include <cassert>
int solveNet(RoutingInst * rst, Net * net) {

}

void AddSegment(Route * route, Segment * segment) {
    
}

int solveRouting(RoutingInst *rst){
    // for each net in the routing instance
    for(int i=0;i<rst->numNets;i++) {
        Net * net = rst->nets + i;
        void * rmst = startRMST(rst, net);
        solveRMST(rmst);
        endRMST(rmst);
    }
    return SUCCESS;
}


