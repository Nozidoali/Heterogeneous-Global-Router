#ifndef UTIL_H
#define UTIL_H

#include "../router.h"

/**
 * int distance(Point * start, Point * end)
 * measure the shortest distance between two points
 */
int distance(Point start, Point end);

/**
 * Segment * lSegment(Point * start, Point * end);
 * Return the L-Shape segment created to connect start and end
 * [Memory Allocated]
 */
Segment * lSegment(RoutingInst * rst, Point start, Point end);
#endif