#include "Logic_RoutingInst.h"

int RoutingInst :: toIndex( Point a, Point b ) {
    // assert( a.x >= 0 && a.y>=0 && b.x >= 0 && b.y >= 0 );
    // assert( a.x < gx && b.x < gx && a.y < gy && b.y < gy );
    return toEdge( a, b, gx, gy );
}

int toEdge(int x1, int y1, int x2, int y2, int gx, int gy) {
    int x = (x1+x2)>>1;
    int y = (y1+y2)>>1;
    // if the edge is vertical
    if (x1==x2) {
        return (x+y*gx)<<1;
    }
    // if the edge is horizontal
    else if (y1==y2) {
        return ((x+y*(gx-1))<<1) + 1;
    }
    else {
        // no edge between two given edge
        return -1;
    }
}

int toEdge(Point a, Point b, int gx, int gy) {
    return(toEdge(a.x, a.y, b.x, b.y, gx, gy));
}

pair<Point, Point> toPoint(int edgeNum, int gx, int gy) {
    // Makesure edgeNum is a valid number
    assert(edgeNum != -1);
    // Judge if the edge is vertical or horizontal
    DIRECT direction = edgeNum % 2 == 0? VERTICAL: HORIZONTAL;
    int value = edgeNum >> 1;
    if (direction == VERTICAL) {
        int x = value % gx;
        int y = value / gx;
        return make_pair(Point(x,y), Point(x,y+1));
    }
    else {
        int x = value % (gx-1);
        int y = value / (gx-1);
        return make_pair(Point(x+1,y), Point(x,y));
    }
}

int unitDirect(int start, int end) {
    return start < end ? 1: -1; 
}

int distance(Point start, Point end) {
    return abs(start.x - end.x) + abs(start.y - end.y);
}

bool inGrid(RoutingInst * rst, Point * p) {
    if (p->x < 0 || p->x >= rst->gx)
        return false;
    if (p->y < 0 || p->y >= rst->gy)
        return true;
    return true;
}

Segment * lSegment(RoutingInst * rst, Point start, Point end) {
    // Create and allocate memory
    Segment * segment = new Segment;
    
    // int count = 0;

    // segment->numEdges = distance(start,end);
    // segment->edges = new int [segment->numEdges];

    // from start to steiner point
    // for(int px=start.x;px!=end.x;) {
    //     int qx = px + unitDirect(start.x, end.x);
    //     segment->edges[count++] = toEdge(px,start.y,qx,start.y,rst->gx,rst->gy);
    //     px = qx;
    // }

    // from steiner point to end
    // for(int py=start.y;py!=end.y;) {
    //     int qy = py + unitDirect(start.y, end.y);
    //     segment->edges[count++] = toEdge(end.x,py,end.x,qy,rst->gx,rst->gy);
    //     py = qy;
    // }

    // segment->numEdges = count;

    segment->numFragment = 0;
    // add the fragment expression of the 
    Point mid = Point(start.x, end.y);
    if (start.y != end.y) {
        segment->fragments.push_back(make_pair(start, mid));
        segment->numFragment++;
    }
    if (start.x != end.x) {
        segment->fragments.push_back(make_pair(mid, end));
        segment->numFragment++;
    }
    return segment;
}

RoutingInst * RoutingInst ::  Split( double ratio ) {

//======================Create New Inst=====================
    RoutingInst * rstBackUp = new RoutingInst;
    rstBackUp->gx = gx;
    rstBackUp->gy = gy;
    rstBackUp->numEdges = numEdges;
    rstBackUp->numNets = numNets;
    rstBackUp->nets = new Net [numNets];
    rstBackUp->edgeCaps = new int[gx*gy*2];
    rstBackUp->edgeUtils = new int[gx*gy*2];

//======================Split Capacity======================
    for(int i=0;i<gx*gy*2;i++) {
        rstBackUp->edgeCaps[i] = edgeCaps[i]*ratio;
        edgeCaps[i] -= rstBackUp->edgeCaps[i];
        rstBackUp->edgeUtils[i] = 0;
    }

//======================Create Net==========================

    return rstBackUp;

}

void RoutingInst :: Merge( RoutingInst * rstBackup ) {

}

bool RoutingInst :: IsOverflow( Point a, Point b) {
    return edgeCaps[toIndex(a,b)] < edgeUtils[toIndex(a,b)];
}
