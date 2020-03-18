#ifndef ROUTINGINST_H
#define ROUTINGINST_H

#include "Basic_Net.h"
#include "Global.h"
#include <cassert>

/**
 * A structure to represent the routing instance
 */
class RoutingInst
{
public:
    int gx ;		/* x dimension of the global routing grid */
    int gy ;		/* y dimension of the global routing grid */

    int cap ;

    int numNets ;	/* number of nets */
    Net *nets ;		/* array of nets */

    int numEdges ; 	/* number of edges of the grid */
    int *edgeCaps; 	/* array of the actual edge capacities after considering for blockages */
    int *edgeUtils;	/* array of edge utilizations */  

};

/**
 * Direction of the edge
 */
enum DIRECT {
    VERTICAL,
    HORIZONTAL
};

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

/**
 * int distance(Point * start, Point * end)
 * measure the shortest distance between two points
 */
int distance(Point start, Point end);
int distance(pair<Point, Point> p);

/**
 * Segment * lSegment(Point * start, Point * end);
 * Return the L-Shape segment created to connect start and end
 * [Memory Allocated]
 */
Segment * lSegment(RoutingInst * rst, Point start, Point end);
#endif