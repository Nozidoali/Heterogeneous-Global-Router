#ifndef SOLVER_H
#define SOLVER_H

#include "../router.h"

/**
 * int solveNet(RoutingInst * rst, Net * net)
 * For the valid rst and the net in it, connect the pins in the net by segments.
 */
int solveNet(RoutingInst * rst, Net * net);


/* int solveRouting(routingInst *rst)
   This function creates a routing solution
   input: pointer to the routing instance
   output: 1 if successful, 0 otherwise (e.g. the data structures are not populated) 
*/
int solveRouting(RoutingInst *rst);

#endif
