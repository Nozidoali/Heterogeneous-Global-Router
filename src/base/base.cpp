#include "base.h"


/**
 * Return the index of edge between two given point.
 */
int toEdge(int x1, int y1, int x2, int y2, int gx, int gy) {
    int x = (x1+x2)>>1;
    int y = (y1+y2)>>1;
    if (x1==x2) {
        return x+y*gx;
    }
    else if (y1==y2) {
        return x*y*(gx-1);
    }
    else {
        // no edge between two given edge
        return -1;
    }
}