// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.
#ifndef BASE_H
#define BASE_H

#include "../router.h"
#include <stdio.h>
#include <utility>

/**
 * int toEdge(int x1, int y1, int x2, int y2, int gx, int gy)
 * From Point ( x1, y1 ) to Point ( x2, y2 )
 * Return the index of edge between two given point.
 */
int toEdge(int x1, int y1, int x2, int y2, int gx, int gy);

/**
 * int toEdge(int x1, int y1, int x2, int y2, int gx, int gy)
 * From Point a to Point b
 * Return the index of edge between two given point.
 */
int toEdge(Point * a, Point * b, int gx, int gy);

/**
 * pair<Point, Point> toPoint(int edgeNum);
 * Convert the edge index to a pair of 2 points. The edge is between point first and point second.
 */
pair<Point, Point> toPoint(int edgeNum, int gx, int gy);

/* int release(routingInst *rst)
   Release the memory for all the allocated data structures. 
   Failure to release may cause memory problems after multiple runs of your program. 
   Need to recursively delete all memory allocations from bottom to top 
   (starting from segments then routes then individual fields within a net struct, 
   then nets, then the fields in a routing instance, and finally the routing instance)

   output: 1 if successful, 0 otherwise 
*/
int release(RoutingInst *rst);

#endif // BASE_H

