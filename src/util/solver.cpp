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
        // return if do not need to connect
        if (net->numPins <= 1) {
            continue;
        }
        net->nroute.numSegs = net->numPins-1;
        net->nroute.segments.reserve(net->nroute.numSegs);
        // for each pair of pins in the net
        for(int j=0;j<net->numPins-1;j++) {
            // create the L-shape segment
            net->nroute.segments.push_back(lSegment(rst, &net->pins[j], &net->pins[j+1]));
        }
    }
    return SUCCESS;
}
