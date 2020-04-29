#include "Route.h"

Route * Rst_Start( bool isDecomposition, bool isOrdered ) {
    Route * route = new Route;
    route->overflow = 0;
    route->isDecomposition = isDecomposition;
    route->isOrdered = isOrdered;
    route->edgeCaps = NULL;
    route->edgeReserveCaps = NULL;
    route->edgeUtils = NULL;
    route->edgeUtils = NULL;
    route->utilExp = NULL;
    return route;
}

void Rst_Stop( Route * route ) {
    for (int i=0;i<route->numNets;i++) {
        Net_Free( route->nets+i );
    }
    delete [] route->nets; route->nets = NULL;
    delete [] route->edgeCaps; route->edgeCaps = NULL;
    delete [] route->edgeUtils; route->edgeUtils = NULL;
    delete [] route->edgeReserveCaps; route->edgeReserveCaps = NULL;
    delete [] route->edgeWeights; route->edgeWeights = NULL;
    Rst_ClearEstimation( route );
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
            route->edgeReserveCaps = new int[route->gx*route->gy*2];
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
                    route->nets[i].pins.reserve( route->nets[i].numPins );
                    route->nets[i].upper = Point( 0, 0 );
                    route->nets[i].lower = Point( route->gx, route->gy );
                    route->nets[i].pTasks = NULL;
                    route->nets[i].edges  = NULL;
                    for (int j=0; j<route->nets[i].numPins; j++) {
                        // read the x,y coordinates of the pin
                        int x, y;
                        finput >> x >> y;
                        route->nets[i].pins.push_back( Point( x, y ) );
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

        for ( auto & edge : *(route->nets[i].edges) ) {
            foutput << Rst_EdgetoPin( route, edge ).first.toString() << "-"; 
            foutput.flush();
            foutput << Rst_EdgetoPin( route, edge ).second.toString() << endl;
            foutput.flush();
        }
        foutput << "!" << endl;
    }
    foutput.close();
    return;
}

bool CMP_NetOverFlowInc ( const Net & a, const Net & b )   { return a.overflow < b.overflow; }
bool CMP_NetAreaInc     ( const Net & a, const Net & b )   { return (a.upper/a.lower) > (b.upper/b.lower); }
bool CMP_NetCostDec     ( const Net & a, const Net & b )   { return a.cost > b.cost; }
bool CMP_NetWirelength  ( const Net & a, const Net & b )   { return Net_GetWirelength(&a) > Net_GetWirelength(&b); }
void Rst_Solve( Route * route ) {

    // Get the appropriate mode and set the parameter
    MODE mode = Rst_GetMode( route );
    Rst_SetMode( route, mode );

    Tmr_Start();

    Rst_PrintHeader ( route );

    auto printStats = [&]( int i ) {
        cout.width(10); cout << "Route : ";
        cout.width(10); cout << i+1 << " / " << route->numNets;
        cout.width(10); cout << "TOF :";
        cout.width(10); cout << route->overflow;
        cout.width(10); cout << "Time: ";
        cout.width(10); cout << Tmr_ToString() << "\r"; fflush(stdout);        
    };

    // Net Ordering
    if ( route->isOrdered ) 
        sort( route->nets, route->nets+route->numNets, CMP_NetAreaInc );

    // Initial Solution
    for (int i=0;i<route->numNets;i++) {
        Rst_SolveNetInitial( route, route->nets+i );
        printStats( i );
        // cerr << i << "," << route->overflow << endl;;
    }

    // Calculate Net Costs
    Rst_InitWeight( route );
    for (int i=0;i<route->numNets;i++) {
        route->nets[i].cost = Rst_EdgesWeight( route, route->nets[i].edges );
    }

    // Net Ordering
    if ( route->isOrdered )
        sort( route->nets, route->nets+route->numNets, CMP_NetCostDec );  
    
    // Rip-up and Reroute
    Tmr_Start();
    if ( route->isOrdered || route->isDecomposition ) {
        cout << endl;
        for (int i=0;i<route->numNets;i++) {

            // break if time is up
            if ( Tmr_TimeLeft() == 0 ) {
                break;
            }
            
            Rst_RerouteNet( route, route->nets+i );
            // cerr << route->numNets+i << "," << route->overflow << endl;
            printStats( i );
        }
    }

    // skip global search if net is not easy
    if ( Rst_GetMode( route) != EASY ) {
        return;
    }

    // Net Ordering
    if ( route->isOrdered )
        sort( route->nets, route->nets+route->numNets, CMP_NetOverFlowInc );  

    // Try Global Search
    Tmr_Start();
    if ( route->isOrdered || route->isDecomposition ) {
        cout << endl;
        bool timeUp = false;
        int maxTime = 10;
        int prev = route->overflow;
        while ( !timeUp && --maxTime ) {
            for (int i=0;i<route->numNets;i++) {

                // break if time is up
                if ( Tmr_TimeLeft() == 0 ) {
                    timeUp = true;
                    break;
                }
                
                Rst_ReduceNet( route, route->nets+i );
                // cerr << route->numNets+i << "," << route->overflow << endl;
                printStats( i );
            }

            // break if improvement is low
            if( route->overflow >= prev - 5 ) {
                break;
            }

            prev = route->overflow;
        }


    }

}
void Rst_SolveNetInitial( Route * route, Net * net ) {

    assert( !Net_HasResult( net ) );
    assert( Net_HasTasks( net ) );

    for( auto & task : *(net->pTasks) ) {

        Tsk_SetDifficulty( task, route->Initial_Max_Difficulty );
        if ( Tsk_GetSize( task ) < route->Initial_Max_TaskScale && Rst_SolveTaskSearch( route, task, true ) ) {
            continue;
        }
        Rst_SolveTaskInitial( route, task );
    }
    Net_CollectResult( net );
    net->overflow = Rst_UpdateUtil( route, net->edges );
    assert( Net_HasResult( net ) );

}

void Rst_SolveNet( Route * route, Net * net ) {

    assert( !Net_HasResult( net ) );
    assert( Net_HasTasks( net ) );

    for( auto & task : *(net->pTasks) ) {
        Tsk_SetDifficulty( task, 3 );
        if ( !Rst_SolveTaskSearch( route, task, 0 ) ) {
            Tsk_SetDifficulty( task, 0 );
            Rst_SolveTaskSearch( route, task, OF_MAX );
        }
        assert( Tsk_HasResult( task ) );
    }
    Net_CollectResult( net );
    net->overflow = Rst_UpdateUtil( route, net->edges );

}

bool Rst_SolveTaskSearch ( Route * route, Task & task, bool isOpt ) {

    assert ( !Tsk_HasResult(task) ); 

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
            return 1;
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
            if ( isOpt && route->edgeCaps[edge] <= route->edgeUtils[edge] ) {
                continue;
            }

            // net cost
            int cost = target.cost - (target.pos^task.end);
            // weight of the edge between target and point
            cost += route->cap;
            cost -= route->edgeCaps[ edge ];
            cost += route->edgeUtils[ edge ];
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
    return 0;
}

Point Rst_SolveTaskInitial_Helper ( Route * route, Task & task, Point start, Point end ) {

    assert( Tsk_HasResult( task ) );
    
    if ( start == end ) {
        return end;
    }

    Point p1 = end * start, p2 = start * end;
    Point t1 = (start^p1) > (start^p2) ? p1 : p2;
    Point t2 = (start^p1) > (start^p2) ? p1 : p2;

    EDGE edge1 = Rst_PintoEdge( route, start, start>>t1 );
    EDGE edge2 = Rst_PintoEdge( route, start, start>>t2 );

    // the first choice are not going to cause overflow
    if ( Rst_EdgeOverflow( route, edge1 ) == 0 ) {
        Tsk_Append( task, edge1 );
        return start>>t1;
    }
    else {
        Tsk_Append( task, edge2 );
        return start>>t2;
    }
    
}

void Rst_SolveTaskInitial ( Route * route, Task & task ) {

    assert( !Tsk_HasResult( task ) );

    task.edges = new EDGES;

    Point start = task.start;
    while ( start != task.end ) {
        start = Rst_SolveTaskInitial_Helper( route, task, start, task.end );
    }

}

void Rst_SolveTaskLSeg ( Route * route, Task & task ) {

    assert( !Tsk_HasResult( task ) );
    task.edges = new EDGES;

    Point & start = task.start;
    Point & end = task.end;
    
    for (Point p = start; p!=start*end; p=p>>(start*end)) {
        Tsk_Append( task, Rst_PintoEdge(route,p,p>>(start*end)) );
    }

    for (Point p = (start*end); p!=end; p=p>>end) {
        Tsk_Append( task, Rst_PintoEdge(route,p,p>>end) );
    }

    assert( Tsk_HasResult( task ) );
}

int Rst_EdgeOverflow ( Route * route, EDGE edge ) {
    return max( 0, route->edgeUtils[edge] - route->edgeCaps[edge] );
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

int Rst_UpdateUtil ( Route * route, EDGES * edges ) {
    int overflow = 0;
    for ( auto & edge : *edges ) {
        if ( route->edgeUtils[ edge ] >= route->edgeCaps[ edge ] ) {
            route->overflow ++;
            overflow ++;
        }
        route->edgeUtils[ edge ] ++;
        route->edgeWeights[ edge ] ++;
    }
    return overflow;
}

int Rst_UpdateUtil ( Route * route, EDGE edge ) {
    int overflow = 0;
    if ( route->edgeUtils[ edge ] >= route->edgeCaps[ edge ] ) {
        route->overflow ++;
        overflow ++;
    }
    route->edgeUtils[ edge ] ++;
    route->edgeWeights[ edge ] ++;
    return overflow;
}


void Rst_CleanUtil ( Route * route ) {

    for ( int i=0;i<2*route->gx*route->gy;i++) {
        route->edgeUtils[ i ] = 0;
    }
    
}

int Rst_EdgesOverflow ( Route * route, EDGES * edges ) {
    
    int overflow = 0;
    EDGES edges_copy = *edges;
    for ( auto & edge : *edges ) {
        overflow += ( Rst_EdgeOverflow( route, edge ) > 0 );
    }
    return overflow;

}

int Rst_ReleaseUtil ( Route * route, EDGES * edges ) {
    int releaseOverflow = 0;
    for ( auto & edge : *edges ) {
        route->edgeUtils[ edge ] --;
        route->edgeWeights[ edge ] --;
        if ( route->edgeUtils[ edge ] >= route->edgeCaps[ edge ] ) {
            releaseOverflow ++;
            route->overflow --;
        }
    }
    return releaseOverflow;
}

int Rst_ReleaseUtil ( Route * route, EDGE edge ) {
    int releaseOverflow = 0;
    route->edgeUtils[ edge ] --;
    route->edgeWeights[ edge ] --;
    if ( route->edgeUtils[ edge ] >= route->edgeCaps[ edge ] ) {
        releaseOverflow ++;
        route->overflow --;
    }
    return releaseOverflow;
}

int Rst_RerouteNet ( Route * route, Net * net ) {
    
    assert( Net_HasResult( net ) );

    // means that net is lost control if not collected
    net->flag = false;

    // clean the utility, store the overflow released
    int oldOverflow = Rst_ReleaseUtil( route, net->edges );

    for( auto & task : *(net->pTasks) ) {

        // skip large task
        if ( Tsk_GetSize( task ) >= route->Reroute_Max_TaskScale ) {
            continue;
        }

        Tsk_CleanResult( task );

        for ( int difficulty = 1; ; difficulty <<= 1 ) {
            Tsk_SetDifficulty( task, difficulty );

            // break if we have a solution
            if ( Rst_SolveTaskSearch( route, task, true ) ) {
                break;
            }

            // break if we have tried enough
            if ( difficulty * Tsk_GetSize( task ) >= route->Reroute_Max_Complexity || difficulty >= route->Reroute_Max_Difficulty ) {
                Tsk_SetDifficulty( task, 8 );
                Rst_SolveTaskSearch( route, task, false );
                break;
            }
        }

        assert( Tsk_HasResult( task ) );
    }
    EDGES * edges = Tsk_CollectResult( net->pTasks );
    int newOverflow = Rst_EdgesOverflow( route, edges );

    // skip update if new result is worse
    if ( oldOverflow <= newOverflow ) {
        Rst_UpdateUtil( route, net->edges );
        Edg_Free( edges ); 
        return -1;
    }

    // update segments, edges and overflow
    Net_CleanResult( net );
    net->edges = edges;
    net->overflow = newOverflow;

    // we get control again
    net->flag = true;

    // update utility and edge weights
    Rst_UpdateUtil( route, net->edges );
    Rst_UpdateWeight( route, net->edges );

    return oldOverflow - newOverflow;
}

int Rst_RerouteTask ( Route * route, Task & task, int limit ) {

    assert ( Tsk_HasResult(task) ); 

    // skip non-overflow
    if ( Rst_EdgesOverflow( route, task.edges ) == 0 ) {
        return 0;
    }

    // skip large task
    if ( Tsk_GetSize( task ) > 2 ) {
        return 0;
    }

    // deal with the limit, auto set to be the estimation
    if ( limit == AUTO ) {
        Point start =   ( (task.start -= task.end) - Point(task.difficulty, task.difficulty) );
        Point end   =   ( (task.start += task.end) + Point(task.difficulty, task.difficulty) );
        start   = ( start   += Point(0,0) );
        end     = ( end     -= Point( route->gx-1, route->gy-1 ) );

        limit   = Rst_GetEstimation( route, start, end );
    }

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
            Tsk_CleanResult( task );
            task.edges = new EDGES;

            // retrace
            for (Point p=task.end;p!=task.start;p=p-dir[mem_dir[Rst_UniquePoint( route, p )]]) {
                Tsk_Append( task, Rst_PintoEdge( route, p, p-dir[mem_dir[Rst_UniquePoint( route, p )]] ) );
            }

            // make sure a valid result is stored
            assert( Tsk_HasResult( task ) );
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
            if ( route->edgeCaps[ edge ] <= route->edgeUtils[ edge ] ) {
                continue;
            }

            // net cost
            int cost = target.cost - (target.pos^task.end);
            // weight of the edge between target and point
            cost += route->cap;
            cost -= route->edgeCaps[ edge ];
            cost += route->edgeUtils[ edge ];
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

    // make sure a valid result is stored
    assert( Tsk_HasResult( task ) );
    return false;
}

void Rst_InitWeight ( Route * route ) {
    for (int i=0;i<2*route->gx*route->gy;i++) {
        route->edgeWeights[i] = 1 + route->edgeUtils[i];
    }
}

void Rst_UpdateWeight ( Route * route, EDGES * edges ) {
    for ( auto & edge : *edges ) {
        route->edgeWeights[ edge ] = route->edgeUtils[ edge ] + 1;
    } 
}

void Rst_PrintStat ( Route * route ) {
    ofstream fout("static.csv");
    fout << "area" << endl;
    for (int i=0;i<route->numNets;i++) {
        fout << route->nets[i].edges->size() << endl;
    }
    fout.close();
}

int Rst_GetOverflow ( Route * route ) {
    int overflow = 0;
    for( int i=0;i<2*route->gx*route->gy;i++ ) {
        overflow += max( 0, route->edgeUtils[i] - route->edgeCaps[i] );
    }
    return overflow;
}

void Rst_PrintNetResult ( Route * route, Net * net ) {

    assert( route && net );
    assert( Net_HasResult( net ) );
    cout << "n" << net->id << endl;
    for ( auto & edge : *( net->edges ) ) {
        cout << edge << endl;
        cout << Rst_EdgetoPin( route, edge ).first.toString()  << "-";
        cout << Rst_EdgetoPin( route, edge ).second.toString() << endl;
    }
}

void Rst_FixEdge ( Route * route, EDGE edge ) {

}

void Rst_FixNet ( Route * route, Net * net ) {
    EDGES::iterator it;
    for ( it=net->edges->begin(); it!=net->edges->end(); it++ ) {
        EDGE edge = *it;

        // continue if already non-overflow
        if ( Rst_EdgeOverflow( route, edge ) <= 0 ) {
            continue;
        }

        Point start = Rst_EdgetoPin( route, edge ).first;
        Point end = Rst_EdgetoPin( route, edge ).second;

        // solve it by A*
        Task task = Tsk_Init( start, end );
        Tsk_SetDifficulty( task, 1 );
        if ( Rst_SolveTaskSearch( route, task, true ) ) {
            assert( Tsk_HasResult( task ) );
            EDGES * solution = Tsk_GetResult( task );
            
            // clean the old result
            Rst_ReleaseUtil( route, *it );
            Net_RemoveEdge( net, it );

            // add the new result
            Net_AddEdges( net, solution );
            Rst_UpdateUtil( route, solution );
        }
        Tsk_CleanResult( task );
    }

    assert( Net_HasResult( net ) );
    
    // clean up all the redundant nodes
    set<Point> pins;

    // record all the pins
    for ( auto point : net->pins ) {
        pins.insert( point );
    }

    // add all the edges in the net
    for ( auto & edge : *( net->edges ) ) {
        Point start = Rst_EdgetoPin( route, edge ).first;
        Point end = Rst_EdgetoPin( route, edge ).second;

        if ( pins.find( start ) != pins.end() || pins.find( end ) != pins.end() ) {
            continue;
        }

        net->edges->erase( edge );
        int count = 0;

        for ( int i=0;i<4;i++ ) {
            Point point = start + dir[i];
            if ( net->edges->count( Rst_PintoEdge( route, start, point ) ) > 0 ) {
                count ++;
                break;
            }
        }
        if ( count == 0 ) {
            continue;
        }
        count = 0;
        for ( int i=0;i<4;i++ ) {
            Point point = end + dir[i];
            if ( net->edges->count( Rst_PintoEdge( route, end, point ) ) > 0 ) {
                count ++;
                break;
            }
        }

        if ( count == 0 ) {
            continue;
        }
        net->edges->insert( edge );
    }

    // 
}

void  Rst_PrintHeader ( Route * route ) {

    string mode[3] = {"EASY","MEDIUM","HARD"};

    cout << "************************************************************" << endl ;
    cout << "*                                                          *" << endl ;
    cout << "*                 Router (Version 3.0)                     *" << endl ;
    cout << "*                  copyright@why                           *" << endl ;
    cout << "*                                                          *" << endl ;
    cout << "************************************************************" << endl ;

    cout << endl;

    cout << " Info: " << endl;
    cout.width(30); cout << "Total Net Number = " << route->numNets << endl;
    cout.width(30); cout << "Total Grid Size  = " << route->gx << " * " << route->gy << endl;
    cout.width(30); cout << "Route Difficulty = " << mode[ (unsigned int)Rst_GetMode( route )  ] << endl;

    cout << endl;
}

void Rst_ReserveCap ( Route * route ) {
    for ( int i=0;i<2*route->gx*route->gy;i++ ) {
        if ( route->edgeCaps[i] > route->edgeUtils[i] ) {
            continue;
        } 
        route->edgeReserveCaps[i] = route->edgeUtils[i] - route->edgeCaps[i];
        route->edgeCaps[i] -= route->edgeReserveCaps[i];
        route->overflow += route->edgeReserveCaps[i];
    }
}
void  Rst_ReleaseCap ( Route * route ) {
    for ( int i=0;i<2*route->gx*route->gy;i++ ) {

        route->overflow -= Rst_EdgeOverflow( route, i ); 
        route->edgeCaps[i] += route->edgeReserveCaps[i];
        route->edgeReserveCaps[i] = 0;
        route->overflow += Rst_EdgeOverflow( route, i ); 

    }
}

void Rst_NetInitTasks ( Route * route, Net * net ) {

    // make sure the tasks are not initialized
    assert( !Net_HasTasks( net ) );

    net->pTasks = route->isDecomposition? Net_CreateTaskMST( net ) : Net_CreateTask( net );
    
    // make sure nets are initialized correctly
    assert( Net_HasTasks( net ) );

}

void Rst_Init ( Route * route ) {

    // initialize task for net
    for ( int i=0;i<route->numNets;i++ ) {
        Rst_NetInitTasks( route, route->nets+i );
    }

    // initialize estimation
    Rst_InitEstimation( route );

}

MODE Rst_GetMode ( Route * route ) {

    if ( route->numNets < 200000 ) {
        // adaptec1
        return MEDIUM;
    }

    if ( route->numNets < 300000 ) {
        // adaptec2
        return EASY;
    }

    return DIFFICULT;
}

void Rst_SetMode ( Route * route, MODE mode ) {
    switch ( mode )
    {
    // Adaptec2 is EASY
    case EASY:
        route->Initial_Max_Difficulty   = 3;
        route->Initial_Max_TaskScale    = 20;
        route->Reroute_Max_Difficulty   = 64;
        route->Reroute_Max_Complexity   = 1024;
        route->Reroute_Max_TaskScale    = 2147483647;
        break;
    
    // Adaptec1 is MEDIUM
    case MEDIUM:
        route->Initial_Max_Difficulty   = 1;
        route->Initial_Max_TaskScale    = 10;
        route->Reroute_Max_Difficulty   = 4;
        route->Reroute_Max_Complexity   = 16;
        route->Reroute_Max_TaskScale    = 20;
        break;
    
    // Adaptec1 is MEDIUM
    case DIFFICULT:
        route->Initial_Max_Difficulty   = 0;
        route->Initial_Max_TaskScale    = 10;
        route->Reroute_Max_Difficulty   = 8;
        route->Reroute_Max_Complexity   = 32;
        route->Reroute_Max_TaskScale    = 20;
        break;
    
    default:
        break;
    }
}

void Rst_PrintPinsDist ( Route * route ) {
    ofstream foutput("pinDist.csv");
    foutput << "x,y" << endl;

    for ( int i=0;i<route->numNets;i++ ) {
        Net * net = route->nets + i;
        for ( auto & pin : net->pins ) {
            foutput << pin.x << "," << pin.y << endl;
        }
    }

    foutput.close();
}

void Rst_PrintWireDist ( Route * route ) {
    ofstream foutput("wireDist.csv");
    foutput << "x,y" << endl;

    for ( int i=0;i<route->numNets;i++ ) {
        Net * net = route->nets + i;
        for ( auto & edge : *(net->edges) ) {
            foutput << Rst_EdgetoPin( route, edge ).first.x << ",";
            foutput << Rst_EdgetoPin( route, edge ).first.y << endl;
            foutput << Rst_EdgetoPin( route, edge ).second.x << ",";
            foutput << Rst_EdgetoPin( route, edge ).second.y << endl;            
        }
    }
}

void Rst_PrintCapsDist ( Route * route ) {
    ofstream foutput("capsDist.csv");
    foutput << "x,y" << endl;

    for ( int edge=0;edge<2*route->gx*route->gy;edge++ ) {
        if ( !Rst_PointIsValid( route, Rst_EdgetoPin( route, edge ).first ) ) {
            continue;
        }
        if ( !Rst_PointIsValid( route, Rst_EdgetoPin( route, edge ).second ) ) {
            continue;
        }
        foutput << Rst_EdgetoPin( route, edge ).first.x << ",";
        foutput << Rst_EdgetoPin( route, edge ).first.y << endl;
        foutput << Rst_EdgetoPin( route, edge ).second.x << ",";
        foutput << Rst_EdgetoPin( route, edge ).second.y << endl;
    }
}

bool Rst_HasEstimation ( Route * route ) {
    return route->utilExp != NULL;
}

void Rst_ClearEstimation ( Route * route ) {
    delete route->utilExp; route->utilExp = NULL;
}

double Rst_GetEstimation ( Route * route, Point start, Point end ) {

    // should have been initialized
    assert( route->utilExp != NULL );

    Point lower = ( start -= end );
    Point upper = ( start += end );

    double exp = 0;
    exp += route->utilExp[ lower.y * route->gx + lower.x ];
    exp += route->utilExp[ upper.y * route->gx + upper.x ];
    exp -= route->utilExp[ lower.y * route->gx + upper.x ];
    exp -= route->utilExp[ upper.y * route->gx + lower.x ];

    return exp;

}

void Rst_InitEstimation ( Route * route ) {

    return;

    // has not been initialized
    assert( route->utilExp == NULL );

    route->utilExp = new double [route->gx * route->gy];
    for ( int i=0;i<route->numNets;i++ ) {

        Net * net = route->nets + i;

        // should be after all tasks are initialized
        assert( Net_HasTasks( net ) );

        for ( auto & task : *(net->pTasks) ) {

            // calculate the estimation
            Point lower = ( task.start -= task.end );
            Point upper = ( task.start += task.end );
            int expTotalWirelength = ( lower ^ upper );
            int TotalArea = Tsk_GetArea( task );
            double expUtil = (double)expTotalWirelength/(double)TotalArea;

            // update prefix sum
            for ( int x=lower.x;x<=upper.x;x++ ) {
                for ( int y=lower.y;y<upper.y;y++ ) {
                    double enclose = (x-lower.x)*(y-lower.y)*expUtil;
                    route->utilExp[y*route->gx+x] += enclose;
                }
            }
        }
    }
}

double  Rst_GetOfEstimation ( Route * route, Point start, Point end ) {

    double utilExp = Rst_GetEstimation( route, start, end );

    int x = abs( start.x - end.x ) + 1;
    int y = abs( start.y - end.y ) + 1;
    int area = 2*x*y - x -y;

    double capsExp = route->cap * area;

    return max( 0.0, utilExp - capsExp );

}

int Rst_ReduceNet ( Route * route, Net * net ) {
   
    assert( Net_HasResult( net ) );

    // skip the net without control
    if ( net->flag == false ) {
        return -1;
    }

    // we are losing control
    net->flag = false;
    
    // clean the utility, store the overflow released
    int oldOverflow = Rst_ReleaseUtil( route, net->edges );

    for( auto & task : *(net->pTasks) ) {

        // skip non-overflow
        if ( Rst_EdgesOverflow( route, task.edges ) == 0 ) {
            continue;
        }

        Tsk_CleanResult( task );
        Tsk_SetDifficulty( task, 100 );
        // break if we have a solution
        if ( !Rst_SolveTaskSearch( route, task, true ) ) {
            Tsk_SetDifficulty( task, 32 );
            Rst_SolveTaskSearch( route, task, false );
        }

        assert( Tsk_HasResult( task ) );
    }
    EDGES * edges = Tsk_CollectResult( net->pTasks );
    int newOverflow = Rst_EdgesOverflow( route, edges );

    // skip update if new result is worse
    if ( oldOverflow <= newOverflow ) {
        Rst_UpdateUtil( route, net->edges );
        Edg_Free( edges ); 
        return -1;
    }

    // update segments, edges and overflow
    Net_CleanResult( net );
    net->edges = edges;
    net->overflow = newOverflow;

    // we get the control back
    net->flag = true;

    // update utility and edge weights
    Rst_UpdateUtil( route, net->edges );
    Rst_UpdateWeight( route, net->edges );

    return oldOverflow - newOverflow;
}
