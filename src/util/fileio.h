#ifndef FILEIO_H
#define FILEIO_H

#include "../router.h"


/* int readBenchmark(const char *fileName, routingInst *rst)
   Read in the benchmark file and initialize the routing instance.
   This function needs to populate all fields of the routingInst structure.
   input1: fileName: Name of the benchmark input file
   input2: pointer to the routing instance
   output: 1 if successful
*/
int readBenchmark(const char *fileName, RoutingInst *rst);

  
/* int writeOutput(const char *outRouteFile, routingInst *rst)
   Write the routing solution obtained from solveRouting(). 
   Refer to the project link for the required output format.

   Finally, make sure your generated output file passes the evaluation script to make sure
   it is in the correct format and the nets have been correctly routed. The script also reports
   the total wirelength and overflow of your routing solution.

   input1: name of the output file
   input2: pointer to the routing instance
   output: 1 if successful, 0 otherwise 
  */
  int writeOutput(const char *outRouteFile, RoutingInst *rst);

#endif