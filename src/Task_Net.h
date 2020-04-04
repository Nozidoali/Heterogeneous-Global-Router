#ifndef TASK_NET_H
#define TASK_NET_H

#include "Logic_RoutingInst.h"
#include "Task_Pins.h"


class NetTask 
{

public:
    NetTask(){ rst = NULL; net = NULL; overflow = 0; wirelength = 0; }
    NetTask( RoutingInst * _rst, Net * _net );
    ~NetTask();
    void Solve( double parameter = 1 );
    void Save();
    void Remove();
    bool isOverflow() { return overflow > 0; }

private:
    int overflow;             // current overflow
    int wirelength;           // current wirelength
    RoutingInst * rst;        // the routing instance
    Net * net;                // the corresponding network
    vector<PinsTask> tasks; // the sub tasks

};

#endif