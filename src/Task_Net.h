#ifndef TASK_NET_H
#define TASK_NET_H

#include "Logic_RoutingInst.h"
#include "Task_Pins.h"


class NetTask
{

private:
    RoutingInst * rst;        // the routing instance
    Net * net;                // the corresponding network
    vector<PinsTask *> tasks; // the sub tasks

public:
    NetTask(){}
    NetTask( RoutingInst * _rst, Net * _net );
    ~NetTask();
    void Solve();
    void Save();
    void Remove();

};

#endif