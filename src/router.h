#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>

#include "util/util.h"
#include "util/fileio.h"
#include "base/base.h"

using namespace std;

/**
 * Functions ./base/base.h
 */
extern int toEdge(int x1, int x2, int y1, int y2, int gx, int gy);

/**
 * Functions ./util/util.h
 */
extern int solveRouting(RoutingInst *rst);
extern int release(RoutingInst *rst);


/**
 * Functions ./util/fileio.h
 */
extern int readBenchmark(const char *fileName, RoutingInst *rst);
extern int writeOutput(const char *outRouteFile, RoutingInst *rst);

#endif