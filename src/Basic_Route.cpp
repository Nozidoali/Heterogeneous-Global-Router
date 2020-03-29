#include "Basic_Route.h"

void Route :: AddSegment(Segment * segment) {
    segments.push_back(segment);
    numSegs++;
}

void Route :: AddSegment(Segment segment) {
    Segment * seg = new Segment;
    *seg = segment;
    segments.push_back(seg);
    numSegs++;
}