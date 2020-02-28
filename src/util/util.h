#ifndef UTIL_H
#define UTIL_H

#include "../base/base.h"
  
/* int solveRouting(routingInst *rst)
   This function creates a routing solution
   input: pointer to the routing instance
   output: 1 if successful, 0 otherwise (e.g. the data structures are not populated) 
*/
int solveRouting(RoutingInst *rst);

  
/* int release(routingInst *rst)
   Release the memory for all the allocated data structures. 
   Failure to release may cause memory problems after multiple runs of your program. 
   Need to recursively delete all memory allocations from bottom to top 
   (starting from segments then routes then individual fields within a net struct, 
   then nets, then the fields in a routing instance, and finally the routing instance)

   output: 1 if successful, 0 otherwise 
*/
 int release(RoutingInst *rst);

#endif