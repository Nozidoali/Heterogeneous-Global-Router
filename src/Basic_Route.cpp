#include "Basic_Route.h"

void Route :: AddSegment(Segment * segment) {
    segments.push_back(segment);
    numSegs++;
}
