#include "Task_Router.h"
#include <thread>

RouterTask :: RouterTask( RoutingInst * _rst ) {
    rst = _rst;
    sort(rst->nets, rst->nets + rst->numNets);
    for(int i=0;i<rst->numNets;i++) {
        tasks.push_back( NetTask( rst, rst->nets+i ) );
    }
}

RouterTask :: ~RouterTask() {
    tasks.clear();
}

void RouterTask :: Solve() {
    for(auto& task : tasks) {
        task.Solve( 1 );
    }
    for(auto& task : tasks) {
        if ( task.isOverflow() ) {
            task.Remove();
            task.Solve( 2 );
        }
        if ( task.isOverflow() ) {
            task.Remove();
            task.Solve( 4 );
        }
    }
    for(auto& task : tasks) {
        if ( !task.isOverflow() ) {
            task.Remove();
            task.Solve( 1 );
        }
        if ( !task.isOverflow() ) {
            task.Remove();
            task.Solve( 2 );
        }
    }
}

void RouterTask :: Save() {
    for(auto& task : tasks) {
        task.Save();
    }
}