#ifndef TASK_PINS_H
#define TASK_PINS_H

#include "Logic_RoutingInst.h"
#include "Logic_Astar.h"
#include <queue>
#include <iostream>
#include <map>
#include <algorithm>
using namespace std; 

class PinsTask 
{

private:
    RoutingInst * rst;      // the routing instance
    Net * net;              // the corresponding network
    Point start, end;       // the start and the end pins of the network
    Segment * segment;      // the current solution of the 2-pins network

    int overflow;           // the current overflow 
    int wirelength;         // the current wirelength

public:
    PinsTask(){ rst = NULL; net = NULL; segment = NULL; }
    PinsTask( RoutingInst * _rst, Net * _net, Point _start, Point _end );
    ~PinsTask();
    Segment * Route( double parameter, bool isOpt );
    void Save();
    void Solve( double parameter = 1 );
    void Remove();
    bool isOverflow()   { return overflow > 0; }
    bool isDone()       { return segment != NULL; }
    int getOverflow()   { return overflow; }
    int getWireLength() { return wirelength; }

};

#endif