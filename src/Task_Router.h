#ifndef TASK_ROUTER_H
#define TASK_ROUTER_H
#include "Logic_RoutingInst.h"
#include "Task_Net.h"

class RouterTask
{

private:
    RoutingInst * rst;      // the routing instance
    vector<NetTask *> tasks;

public:
    RouterTask(){}
    RouterTask( RoutingInst * rst );
    ~RouterTask();

    void Solve();
    void Save();
};

#endif