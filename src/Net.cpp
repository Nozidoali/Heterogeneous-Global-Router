#include "Net.h"

#include "Logic_UnionFind.h"
Tasks * Net_CreateTask( Net * net ) {

    Tasks * ptasks = new Tasks;
    
    // without MST algorithm
    for (int i=0;i<net->numPins-1;i++) {
        Task task = Tsk_Init( net->pins[i], net->pins[i+1] );
        ptasks->push_back( task );
    }
    return ptasks;
}
Tasks * Net_CreateTaskMST( Net * net ) {

    Tasks * ptasks = new Tasks;

    struct Edge {
        Point a,b;
        int ia,ib;
        bool operator < (const Edge e) const { return (a^b) > (e.a^e.b); };
        Edge(Point _a, Point _b, int _ia, int _ib) {
            a = _a; b = _b; ia = _ia; ib = _ib;
        }
    };
    // Declare a priority queue to maintain the MST
    priority_queue<Edge> queue;

    // Add all the possible edges to the queue
    for(int i=0;i<net->numPins;i++) {
        for(int j=i+1;j<net->numPins;j++) {
            queue.push(Edge(net->pins[i], net->pins[j], i, j));
        }
    }

    UnionFind uf(net->numPins);
    while(!queue.empty()) {
        Edge e = queue.top();
        queue.pop();
        if (uf.isMerged(e.ia,e.ib)) {
            // continue if we have already connected them
            continue;
        }
        else {

            // merge to indicate that we have already connected a and b
            uf.Merge(e.ia,e.ib);

            Point a = e.a;
            Point b = e.b;

// =================================Start===============================
            Task task = Tsk_Init( a, b );
            ptasks->push_back( task );
// =================================End=================================
        }
    }

    assert( ptasks->size() == (unsigned int)net->numPins - 1 );
    return ptasks;
}

void Net_CollectResult( Net * net, Tasks * tasks ) {

    assert( !Net_HasResult( net ) );
    net->edges = Tsk_CollectResult( tasks );

}

int Net_GetWirelength( const Net * net ) {
    return (int)net->edges->size();
}

bool Net_HasResult( const Net * net ) {
    return net->edges != NULL;
}

void Net_CleanResult( Net * net ) {
    net->edges->clear();
    delete net->edges;
    net->edges = NULL;
}

void Net_Free( Net * net ) {
    if ( net == NULL )
        return;
    if ( Net_HasResult( net ) ) {
        Net_CleanResult( net );
    }
    delete [] net->pins; net->pins = NULL;
}

int Net_GetOverflow( const Net * net ) {
    return net->overflow;
}

int Net_GetArea ( const Net * net ) {
    return net->lower / net->upper;
}
