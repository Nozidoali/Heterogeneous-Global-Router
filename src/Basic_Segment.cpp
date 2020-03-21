#include "Basic_Segment.h"

Segment :: Segment( Point _a, Point _b ) {
    p1 = _a; p2 = _b;
    numEdges = 0;
    numFragment = 0;
}

void Segment :: AddFragment( Point a, Point b ) {
    numFragment++;
    fragments.push_back( make_pair(a,b) );
}

