#include "base.h"

#include <cassert>
int toEdge(int x1, int y1, int x2, int y2, int gx, int gy) {
    int x = (x1+x2)>>1;
    int y = (y1+y2)>>1;
    // if the edge is vertical
    if (x1==x2) {
        return (x+y*gx)<<1;
    }
    // if the edge is horizontal
    else if (y1==y2) {
        return (x*y*(gx-1))<<1 + 1;
    }
    else {
        // no edge between two given edge
        return -1;
    }
}

int toEdge(Point * a, Point * b, int gx, int gy) {
    return(toEdge(a->x, a->y, b->x, b->y, gx, gy));
}

pair<Point, Point> toPoint(int edgeNum, int gx, int gy) {
    // Makesure edgeNum is a valid number
    assert(edgeNum != -1);
    // Judge if the edge is vertical or horizontal
    DIRECT direction = edgeNum & 1 == 1? VERTICAL: HORIZONTAL;
    int value = edgeNum >> 1;
    if (direction == VERTICAL) {
        int x = value % gx;
        int y = value / gx;
        return make_pair(Point(x,y), Point(x,y+1));
    }
    if (direction == HORIZONTAL) {
        int x = value % (gx-1);
        int y = value / (gx-1);
        return make_pair(Point(x+1,y), Point(x,y));
    }
}

int release(RoutingInst *rst) {
    delete [] rst->edgeCaps;
    delete [] rst->edgeUtils;
    for (int i=0;i<rst->numNets;i++) {
        delete [] rst->nets[i].pins;
    }
    delete [] rst->nets;
    delete rst;
    return SUCCESS;
}