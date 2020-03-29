#ifndef INTERFACE_H
#define INTERFACE_H

#include <fstream>
#include <cstring>
#include <cassert>
using namespace std;
#include "Logic_RoutingInst.h"
#include "Solver_rmst.h"
#include "Solver_Weighted.h"
#include "Task_Router.h"
#include "Global.h"

// #define SOLVER_WEIGHTED

class Solver 
{

private:
    RoutingInst * rst;
public:
    Solver();
    virtual ~Solver() = 0;
    RoutingInst * GetInst();
    virtual int Solve() = 0;

};
const ifstream operator >> ( ifstream finput, Solver * solver );
ofstream operator << ( ofstream foutput, Solver * solver );

/* int release(routingInst *rst)
   Release the memory for all the allocated data structures. 
   Failure to release may cause memory problems after multiple runs of your program. 
   Need to recursively delete all memory allocations from bottom to top 
   (starting from segments then routes then individual fields within a net struct, 
   then nets, then the fields in a routing instance, and finally the routing instance)

   output: 1 if successful, 0 otherwise 
*/
int release(RoutingInst *rst);


/* int readBenchmark(const char *fileName, routingInst *rst)
   Read in the benchmark file and initialize the routing instance.
   This function needs to populate all fields of the routingInst structure.
   input1: fileName: Name of the benchmark input file
   input2: pointer to the routing instance
   output: 1 if successful
*/
int readBenchmark(const char *fileName, RoutingInst *rst);

/* int solveRouting(routingInst *rst)
   This function creates a routing solution
   input: pointer to the routing instance
   output: 1 if successful, 0 otherwise (e.g. the data structures are not populated) 
*/
int solveRouting(RoutingInst *rst);
  
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