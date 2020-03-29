#include "Task_Router.h"

RouterTask :: RouterTask( RoutingInst * _rst ) {
    rst = _rst;

#ifndef DEBUG
    cout << "======Initializing Tasks======" << endl;
#endif

    tasks.reserve( rst->numNets );

    for(int i=0;i<rst->numNets;i++) {
        tasks.push_back( new NetTask( rst, rst->nets+i ) );

#ifndef DEBUG
        cout << i+1 << " / " << rst->numNets << "\r";
        fflush(stdout);
#endif
    }

#ifndef DEBUG
    cout << endl;
#endif
}

RouterTask :: ~RouterTask() {
    tasks.clear();
}

void RouterTask :: Solve() {

#ifndef DEBUG
    cout << "======Routing Tasks======" << endl;
#endif

    for(int i=0;i<rst->numNets;i++) {
        tasks[i]->Solve();

#ifndef DEBUG
        cout << i+1 << " / " << rst->numNets << "\r";
        fflush(stdout);
#endif
    }

#ifndef DEBUG
    cout << "======Clearing Tasks======" << endl;
#endif

    for(int i=0;i<rst->numNets;i++) {
        tasks[i]->Remove();

#ifndef DEBUG
        cout << i+1 << " / " << rst->numNets << "\r";
        fflush(stdout);
#endif
    }

#ifndef DEBUG
    cout << "======ReRouting Tasks======" << endl;
#endif

    for(int i=0;i<rst->numNets;i++) {
        tasks[i]->Solve();

#ifndef DEBUG
        cout << i+1 << " / " << rst->numNets << "\r";
        fflush(stdout);
#endif
    }

#ifndef DEBUG
    cout << endl;
#endif
}

void RouterTask :: Save() {

#ifndef DEBUG
    cout << "======Saving Solutions======" << endl;
#endif

    for(int i=0;i<rst->numNets;i++) {
        tasks[i]->Save();

#ifndef DEBUG
        cout << i+1 << " / " << rst->numNets << "\r";
        fflush(stdout);
#endif
    }

#ifndef DEBUG
    cout << endl;
#endif
}