#include "Route.h"

Route * Rst_Start( bool isDecomposition, bool isOrdered ) {
    Route * route = new Route;
    route->isDecomposition = isDecomposition;
    route->isOrdered = isOrdered;
    return route;
}

void Rst_Stop( Route * route ) {
    for (int i=0;i<route->numNets;i++) {
        Net_Free( route->nets+i );
    }
    delete [] route->nets; route->nets = NULL;
    delete [] route->edgeCaps; route->edgeCaps = NULL;
    delete [] route->edgeUtils; route->edgeUtils = NULL;
    delete [] route->edgeWeights; route->edgeWeights = NULL;
    delete route; route = NULL;
}


int Rst_PintoEdge( Route * route, Point a, Point b ) {
    
    assert( (a^b) == 1 );
    int x = (a.x+b.x)>>1;
    int y = (a.y+b.y)>>1;
    // if the edge is vertical
    if ( a.x==b.x ) { return y+x*route->gy+route->gy*route->gx; }
    // if the edge is horizontal
    else { return x+y*route->gx; }

}
SEGMENT Rst_EdgestoPin( Route * route, int edgeleft, int edgeright ) {
    SEGMENT start = Rst_EdgetoPin( route, edgeleft );
    SEGMENT end   = Rst_EdgetoPin( route, edgeright );

    return start.first, end.second;
}
SEGMENT Rst_EdgetoPin( Route * route, int edge ) {

    assert( edge >= 0 && edge < 2*route->gx*route->gy );
    // if the edge is vertical
    if ( edge >= route->gx*route->gy ) {
        int y = ( edge - route->gx*route->gy ) % route->gy;
        int x = ( edge - route->gx*route->gy ) / route->gy;
        return Point(x,y), Point(x,y+1);
    }
    // if the edge is horizontal
    else {
        int x = edge % route->gx;
        int y = edge / route->gx;
        return Point(x,y), Point(x+1,y);
    }

}

void Rst_ReadFile( Route * route, const char * inputFilename ) {
    ifstream finput( inputFilename );
    // file not opened correctly
    if (!finput.is_open()) {
        throw Exception();
        return;
    }
    string command;
    while (!finput.eof()) {
        finput >> command;
        if (command == "grid") {
            // read the x,y scale of the grid
            finput >> route->gx >> route->gy;
            route->numEdges = 2*route->gx*route->gy - route->gx - route->gy;
            route->edgeCaps = new int[route->gx*route->gy*2];
            route->edgeUtils = new int[route->gx*route->gy*2];
            route->edgeWeights = new int[route->gx*route->gy*2];
        }
        else if (command == "capacity") {
            // read the default capacity of each edge
            finput >> route->cap;
            for (int i=0;i<route->gx*route->gy*2;i++) {
                route->edgeCaps[i] = route->cap;
                route->edgeUtils[i] = 0;
                route->edgeWeights[i] = 1;
            }
        }
        else if (command == "num") {
            finput >> command;
            if (command == "net") {
                // read the number of nets
                finput >> route->numNets;
                route->nets = new Net[route->numNets];
                for (int i=0; i<route->numNets; i++) {
                    route->nets[i].id = i;
                    finput >> command;
                    // read the number of pins in the net
                    finput >> route->nets[i].numPins;
                    route->nets[i].pins = new Point[route->nets[i].numPins];
                    route->nets[i].upper = Point( 0, 0 );
                    route->nets[i].lower = Point( route->gx, route->gy );
                    for (int j=0; j<route->nets[i].numPins; j++) {
                        // read the x,y coordinates of the pin
                        finput >> route->nets[i].pins[j].x >> route->nets[i].pins[j].y;
                        route->nets[i].lower = (route->nets[i].lower -=  route->nets[i].pins[j]); 
                        route->nets[i].upper = (route->nets[i].upper +=  route->nets[i].pins[j]); 
                    }
                }
                // read the blockages
                int blockages;
                finput >> blockages;
                for (int i=0, x1, y1, x2, y2, c;i<blockages;i++) {
                    finput >> x1 >> y1 >> x2 >> y2 >> c;
                    int index = Rst_PintoEdge( route, Point(x1,y1), Point(x2,y2) );
                    assert(index>=0 && index<route->gx*route->gy*2);
                    route->edgeCaps[index] = c;
                }
            }
            else {
                finput.close();
                throw Exception();
                return;
            }
        }
        else {
            continue;
        }
    }
    finput.close();
    return;
}

void Rst_WriteFile ( Route * route, const char * outputFileName ) {
    ofstream foutput ( outputFileName );
    for (int i=0;i<route->numNets;i++) {
        // print the node ID in the first line
        foutput << "n" << route->nets[i].id << endl;
        assert( Net_HasResult( route->nets+i ) );

        for ( auto & seg : route->nets[i].segments ) {
            foutput << seg << endl;
        }

/*
        EDGES * edges = route->nets[i].edges;
        for (unsigned int i=0;i<edges->size();i++) {
            EDGE start = (*edges)[i];
            while ( i+1 < edges->size() && (*edges)[i]+1 == (*edges)[i+1] ) { i++; }
            EDGE end = (*edges)[i];
            // print the edge in the correct format
            foutput << Rst_EdgestoPin( route, start, end ).first.toString() << "-";
            foutput << Rst_EdgestoPin( route, start, end ).second.toString() << endl;
        }
*/

        foutput << "!" << endl;
    }
    return;
}

bool CMP_NetOverFlow ( const Net & a, const Net & b ) { return a.overflow > b.overflow; }
bool CMP_NetArea ( const Net & a, const Net & b ) { return (a.upper/a.lower) < (b.upper/b.lower); }
bool CMP_NetCost ( const Net & a, const Net & b ) { return a.cost > b.cost; }
void Rst_Solve( Route * route ) {

    // Initial Solution
    for (int i=0;i<route->numNets;i++) {
        cout << "Route : " << i+1 << " / " << route->numNets << "\r"; fflush(stdout);
        Rst_SolveNetInitial( route, route->nets+i );
    }

    // Calculate Net Costs
    Rst_InitWeight( route );
    for (int i=0;i<route->numNets;i++) {
        route->nets[i].cost = Rst_EdgesWeight( route, route->nets[i].edges );
    }

    // Net Ordering
    if ( route->isOrdered ) 
        sort( route->nets, route->nets+route->numNets, CMP_NetCost );

    // Rip-up and Reroute
    if ( route->isOrdered || route->isDecomposition ) {
        cout << endl;
        for (int i=0;i<route->numNets;i++) {
            cout << "RRR net: " << i+1 << " / " << route->numNets << "\r"; fflush(stdout); 
            Rst_RerouteNet( route, route->nets+i );
        }
    }

}
void Rst_SolveNetInitial( Route * route, Net * net ) {

    assert( !Net_HasResult( net ) );
    Tasks * pTasks = route->isDecomposition? Net_CreateTaskMST( net ) : Net_CreateTask( net );
    for( auto & task : *pTasks ) {
        Rst_SolveTaskInitial( route, task );
        Rst_UpdateSegment( route, net, task.edges );
    }
    Net_CollectResult( net, pTasks );
    net->overflow = Rst_EdgesOverflow( route, net->edges );
    Rst_UpdateUtil( route, net->edges );
    Tsk_Free( pTasks );

}

void Rst_SolveNet( Route * route, Net * net ) {

    assert( !Net_HasResult( net ) );
    Tasks * pTasks = route->isDecomposition? Net_CreateTaskMST( net ) : Net_CreateTask( net );
    for( auto & task : *pTasks ) {
        Tsk_SetDifficulty( task, 3 );
        if ( !Rst_SolveTaskSearch( route, task, true ) ) {
            Tsk_SetDifficulty( task, 0 );
            Rst_SolveTaskSearch( route, task, false );
        }
        Rst_UpdateSegment( route, net, task.edges );
        assert( Tsk_HasResult( task ) );
    }
    Net_CollectResult( net, pTasks );
    net->overflow = Rst_EdgesOverflow( route, net->edges );
    Rst_UpdateUtil( route, net->edges );
    Tsk_Free( pTasks );

}

bool Rst_SolveTaskSearch ( Route * route, Task & task, bool isOpt ) {
    struct Stage {
        int cost;
        Point pos;
        Stage( int cost, Point pos ) : cost(cost), pos(pos) {}
    };
    struct CMP {
        bool operator () ( const Stage & a,  const Stage & b ) { return a.cost > b.cost; }
    };

    map<UNIQUE_POINT, bool> visited;   visited.clear();
    map<UNIQUE_POINT, int> cur_cost;   cur_cost.clear();
    map<UNIQUE_POINT, DIRECT> mem_dir; mem_dir.clear();
    priority_queue<Stage, vector<Stage>, CMP > queue;

    // set a to be the start point
    queue.push( Stage( task.start^task.end, task.start ) );
    cur_cost[ Rst_UniquePoint( route, task.start ) ] = task.start^task.end;
    mem_dir [ Rst_UniquePoint( route, task.start ) ] = NONE;
    // loop until finding end
    while( !queue.empty() ) {
        Stage target = queue.top();
        queue.pop();
        // continue if point has been traversed
        if ( visited.find( Rst_UniquePoint( route, target.pos ) ) != visited.end() ) {
            continue;
        }
        visited[ Rst_UniquePoint( route, target.pos ) ] = true;
        // quit if reach the end point
        if (target.pos == task.end) {
            task.edges = new EDGES;
            // retrace
            for (Point p=task.end;p!=task.start;p=p-dir[mem_dir[Rst_UniquePoint( route, p )]]) {
                Tsk_Append( task, Rst_PintoEdge( route, p, p-dir[mem_dir[Rst_UniquePoint( route, p )]] ) );
            }
            sort(task.edges->begin(), task.edges->end());
            task.edges->erase(unique(task.edges->begin(), task.edges->end()), task.edges->end());
            return true;
        }
        for(unsigned int i=0;i<4;i++) {
            Point point = target.pos + dir[i];
            EDGE edge = Rst_PintoEdge( route, point, target.pos );

            // continue if point is out of range
            if ( !Rst_PointIsValid( route, point ) || !Tsk_PointIsValid( task, point ) ) {
                continue;
            }
            // continue if point is visited
            if ( visited.find( Rst_UniquePoint( route, point ) ) != visited.end() ) {
                continue;
            }
            // continue if edge is out of capacity
            if ( isOpt && Rst_EdgeOverflow( route, edge ) > 0 ) {
                continue;
            }
            // net cost
            int cost = target.cost - (target.pos^task.end);
            // weight of the edge between target and point
            cost += route->edgeWeights[ edge ];
            // weight of estimate distance
            cost += point^task.end;

            if ( 
                    cur_cost.find( Rst_UniquePoint( route, point ) ) == cur_cost.end() ||
                  cost < cur_cost[ Rst_UniquePoint( route, point ) ] 
            ) {
                queue.push( Stage( cost, point ) );
                cur_cost[ Rst_UniquePoint( route, point ) ] = cost;
                mem_dir [ Rst_UniquePoint( route, point ) ] = (DIRECT)i;
            }
        }
    }
    return false;
}

void Rst_SolveTaskInitial ( Route * route, Task & task ) {

    assert( task.edges == NULL );
    task.edges = new EDGES;

    Point & start = task.start;
    Point & end = task.end;
    
    for (Point p = start; p!=start*end; p=p>>(start*end)) {
        Tsk_Append( task, Rst_PintoEdge(route,p,p>>(start*end)) );
    }

    for (Point p = (start*end); p!=end; p=p>>end) {
        Tsk_Append( task, Rst_PintoEdge(route,p,p>>end) );
    }

    sort(task.edges->begin(), task.edges->end());
    task.edges->erase(unique(task.edges->begin(), task.edges->end()), task.edges->end());

    assert( Tsk_HasResult( task ) );
}

int Rst_EdgeOverflow ( Route * route, EDGE edge ) {
    return max( 0, route->edgeUtils[edge] + 1 - route->edgeCaps[edge] );
}

int Rst_EdgeWeight ( Route * route, EDGE edge ) {
    return route->edgeWeights[edge];
}

int Rst_EdgesWeight ( Route * route, EDGES * edges ) {
    int weight = 0;
    for ( auto & edge : *edges ) {
        weight += Rst_EdgeWeight( route, edge );
    }
    return weight;
}


bool Rst_PointIsValid( Route * route, Point point ) {
    return ( point >= Point(0,0) ) && ( point <= Point( route->gx-1,route->gy-1 ) ); 
}

UNIQUE_POINT Rst_UniquePoint ( Route * route, Point a ) {
    return a.x + a.y * route->gy;
}

void Rst_UpdateUtil ( Route * route, EDGES * edges ) {
    for ( auto & edge : *edges ) {
        route->edgeUtils[ edge ] ++;
        route->edgeWeights[ edge ] ++;
    }
}

void Rst_CleanUtil ( Route * route ) {

    for ( int i=0;i<2*route->gx*route->gy;i++) {
        route->edgeUtils[ i ] = 0;
    }
    
}

void Rst_UpdateSegment ( Route * route, Net * net, EDGES * edges ) {

    for (unsigned int i=0;i<edges->size();i++) {
        EDGE start = (*edges)[i];
        while ( i+1 < edges->size() && (*edges)[i]+1 == (*edges)[i+1] ) { i++; }
        EDGE end = (*edges)[i];
        // print the edge in the correct format
        net->segments.push_back( Rst_EdgestoPin( route, start, end ) );
    }

}

int Rst_EdgesOverflow ( Route * route, EDGES * edges ) {
    
    int overflow = 0;
    for ( auto & edge : *edges )
        overflow += ( Rst_EdgeOverflow( route, edge ) > 0 );
    return overflow;

}

int Rst_ReleaseUtil ( Route * route, EDGES * edges ) {
    int releaseOverflow = 0;
    for ( auto & edge : *edges ) {
        if ( route->edgeUtils[ edge ] >= route->edgeCaps[ edge ] )
            releaseOverflow ++;
        route->edgeUtils[ edge ] --;
    }
    return releaseOverflow;
}

int Rst_RerouteNet ( Route * route, Net * net ) {
    
    assert( Net_HasResult( net ) );
    
    // skip large nets
    if ( Net_GetArea( net ) >= 1000 )
        return 0;
    int oldOverflow = Rst_EdgesOverflow( route, net->edges );

    // clean the utility, store the overflow released
    oldOverflow += Rst_ReleaseUtil( route, net->edges );

    Tasks * pTasks = route->isDecomposition? Net_CreateTaskMST( net ) : Net_CreateTask( net );
    for( auto & task : *pTasks ) {
        Tsk_SetDifficulty( task, 0 );
        if ( !Rst_SolveTaskSearch( route, task, true ) ) {
            Rst_SolveTaskInitial( route, task );
        }
        assert( Tsk_HasResult( task ) );
    }
    EDGES * edges = Tsk_CollectResult( pTasks );
    int newOverflow = Rst_EdgesOverflow( route, edges );

    // skip update if new result is worse
    if ( oldOverflow < newOverflow ) {
        Rst_UpdateUtil( route, net->edges );
        Edg_Free( edges ); 
        Tsk_Free( pTasks );
        return -1;
    }

    // update segments, edges and overflow
    Net_CleanResult( net );
    for( auto & task : *pTasks ) { Rst_UpdateSegment( route, net, task.edges ); }
    net->edges = edges;
    net->overflow = newOverflow;

    // update utility
    Rst_UpdateUtil( route, net->edges );

    Tsk_Free( pTasks );
    return oldOverflow - newOverflow;
}

int Rst_RerouteTask ( Route * route, Task & task ) {

}

void Rst_InitWeight ( Route * route ) {
    for (int i=0;i<2*route->gx*route->gy;i++) {
        route->edgeWeights[i] = 1 + route->edgeUtils[i];
    }
}
