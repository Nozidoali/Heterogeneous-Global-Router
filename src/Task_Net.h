#ifndef TASK_NET_H
#define TASK_NET_H

#include "Basic_RoutingInst.h"
#include "Task_Pins.h"

class NetTask 
{

public:
    NetTask(){ rst = NULL; net = NULL; overflow = 0; wirelength = 0; }
    NetTask( RoutingInst * _rst, Net * _net );
    ~NetTask();
    void Solve();
    void Save();
    void SaveUtil();
    void Remove();
    void RemoveUtil();

    bool operator < ( const NetTask & task ) { return area < task.area; }

//==============================Attributes================================== //
    int getOverflow()   { return overflow; }
    int getWireLength() { return wirelength; }
    bool isOverflow()   { return overflow > 0; }
    Net * getNet()      { return net; }

private:
    int x_lower, x_upper, y_lower, y_upper, area; // the boundary of pins
    int overflow;               // current overflow
    int wirelength;             // current wirelength
    RoutingInst * rst;          // the routing instance
    Net * net;                  // the corresponding network
    vector<PinsTask> tasks;     // the sub tasks

//================================Functions================================= //
    int getMaxDifficulty();
};

#endif