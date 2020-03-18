#ifndef SEGMENT_H
#define SEGMENT_H

#include "Basic_Point.h"

/**
 * A structure to represent a segment
 */
class Segment
{
public:
    Point p1 ; 	/* start point of a segment */
    Point p2 ; 	/* end point of a segment */

    int numEdges ; 	/* number of edges in the segment*/
    int *edges ;  	/* array of edges representing the segment*/

    int numFragment; /* number of fragments in the segment*/
    vector<pair<Point, Point> > fragments; /* array of fragments in the segment*/
};

#endif