#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "Basic_Segment.h"
using namespace std;

/**
 * A structure to represent a route
 */
class Route
{
public:
    int numSegs ;  	/* number of segments in a route*/
    vector<Segment *> segments ;  /* an array of segments (note, a segment may be flat, L-shaped or any other shape, based on your preference */
    void AddSegment(Segment * segment);
    void AddSegment(Segment segment);
    Route(){ numSegs = 0; }
};

#endif