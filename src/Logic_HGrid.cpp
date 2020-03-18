#include "Logic_HGrid.h"

hGrid::hGrid(RoutingInst * rst, Net * net) {
    // constructing hGrid
    mem_x = new int [net->numPins];
    mem_y = new int [net->numPins];
    for(int i=0;i<net->numPins;i++) {
        mem_x[i] = net->pins[i].x;
        mem_y[i] = net->pins[i].y;
    }
    // Clean the redundant by sort and unique
    sort(mem_x, mem_x+net->numPins);
    sort(mem_y, mem_y+net->numPins);
    gx = unique(mem_x, mem_x+net->numPins) - mem_x;
    gy = unique(mem_y, mem_y+net->numPins) - mem_y;

    // remember the index of each value
    for(int i=0;i<gx;i++) {
        mem_ix[mem_x[i]] = i;
    }
    for(int i=0;i<gy;i++) {
        mem_iy[mem_y[i]] = i;
    }
    // cout << "gx: " << gx << endl;
    // cout << "gy: " << gy << endl;
}

Point hGrid :: toIndex(Point p) {
    return Point(mem_ix[p.x], mem_iy[p.y]);
}
Point hGrid :: toPoint(Point p) {
    return Point(mem_x[p.x], mem_y[p.y]);
}

hGrid::~hGrid() {
    delete [] mem_x;
    delete [] mem_y;
}