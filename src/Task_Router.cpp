#include "Task_Router.h"
#include <thread>

RouterTask :: RouterTask( RoutingInst * _rst ) {
    rst = _rst;
    for(int i=0;i<rst->numNets;i++) {
        tasks.push_back( NetTask( rst, rst->nets+i ) );
        cout << i+1 << " / " << rst->numNets << "\r"; fflush(stdout);
    }
    cout << endl;
}

RouterTask :: ~RouterTask() {
    tasks.clear();
}

void RouterTask :: Solve() {

    sort( tasks.begin(), tasks.end() );
    int process = 0;
    for ( auto& task : tasks ) {
        
        // solve the whole network
        task.Solve();

        // save the utility in rst
        task.SaveUtil();

#ifdef DEBUG
        cout.width(10);
        cout << "-- net " << task.getNet()->id;
        cout.width(10);
        cout << "OF: " << task.getOverflow();
        cout.width(10);
        cout << "WL: " << task.getWireLength() << endl;
#endif
        // report process
        cout << ++process << " / " << rst->numNets << "\r"; fflush(stdout);
        
    }

    for ( auto& task : tasks ) {
        
        // remove the original result
        task.Remove();

        // solve the whole network
        task.Solve();

        // save the utility in rst
        task.SaveUtil();

#ifdef DEBUG
        cout.width(10);
        cout << "-- net " << task.getNet()->id;
        cout.width(10);
        cout << "OF: " << task.getOverflow();
        cout.width(10);
        cout << "WL: " << task.getWireLength() << endl;
#endif
        // report process
        cout << ++process << " / " << rst->numNets << "\r"; fflush(stdout);
        
    }

}

void RouterTask :: Save() {
    for ( auto& task : tasks ) {
        task.Save();
    }
}