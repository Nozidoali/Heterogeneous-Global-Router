#ifndef TASK_PINS_H
#define TASK_PINS_H

#include "Basic_RoutingInst.h"
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
    int difficulty;         // the difficulty for non-overlap route

public:

//=================================Constructor============================== //
    PinsTask(){ rst = NULL; net = NULL; segment = NULL; }
    PinsTask( RoutingInst * _rst, Net * _net, Point _start, Point _end );
    ~PinsTask();

//====================================Save================================== //
    void Save();
    void SaveUtil();

//====================================Solve================================= //
    Segment * Route( double parameter, bool isOpt = false );
    void Solve();
    void TryHarder();

//====================================Clean================================= //
    void Clean();
    void Remove();

//=================================Information============================== //
    bool isOverflow()   const   { return overflow > 0; }
    bool isDone()       const   { return segment != NULL; }
    int getOverflow()   const   { return overflow; }
    int getWireLength() const   { return wirelength; }
    int getArea()       const   { return abs(start.x-end.x)*(start.y-end.y); }
    int getDifficulty() const   { return difficulty; }

    Segment * getSolution() const { return segment; }

//=================================Operators================================ //
    bool operator < ( const PinsTask & task ) const { return getArea() < task.getArea(); }

};

#endif