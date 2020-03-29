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

public:
    PinsTask(){}
    PinsTask( RoutingInst * _rst, Net * _net, Point _start, Point _end );
    ~PinsTask();
    Segment * Route( double parameter, bool isOpt );
    void Save();
    void Solve();
    void Remove();

};

#endif