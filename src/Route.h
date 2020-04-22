#ifndef ROUTE_H
#define ROUTE_H

#include "Net.h"
#include "Logic_Timer.h"

typedef int UNIQUE_POINT;
struct Route 
{

    int gx ;		/* x dimension of the global routing grid */
    int gy ;		/* y dimension of the global routing grid */

    int cap ;

    int numNets ;	/* number of nets */
    Net * nets ;	/* array of nets */

    int numEdges ; 	/* number of edges of the grid */
    int *edgeCaps; 	/* array of the actual edge capacities after considering for blockages */
    int *edgeUtils;	/* array of edge utilizations */  
    int *edgeWeights;

    bool isDecomposition;
    bool isOrdered;

    int overflow;
};

//==================================Memory Control===========================//
Route *             Rst_Start       ( bool isDecomposition, bool isOrdered );
void                Rst_Stop        ( Route * route );

//==================================Index Convertion=========================//
int                 Rst_PintoEdge   ( Route * route, Point a, Point b );
UNIQUE_POINT        Rst_UniquePoint ( Route * route, Point a );
pair<Point, Point>  Rst_EdgetoPin   ( Route * route, int edge );
pair<Point, Point>  Rst_EdgestoPin  ( Route * route, int edge1, int edge2 );

//==================================File I/O=================================//
void                Rst_ReadFile        ( Route * route, const char * inputFileName );
void                Rst_WriteFile       ( Route * route, const char * outputFileName );

//==================================Solver===================================//
void                Rst_Solve           ( Route * route );
void                Rst_SolveNet        ( Route * route, Net * net );
void                Rst_SolveNetInitial ( Route * route, Net * net );
void                Rst_SolveTaskInitial( Route * route, Task & task );
void                Rst_SolveTaskLSeg   ( Route * route, Task & task );
bool                Rst_SolveTaskSearch ( Route * route, Task & task, bool isOpt );

//==================================RRR======================================//
int                 Rst_RerouteNet      ( Route * route, Net * net );
int                 Rst_RerouteTask     ( Route * route, Task & net );

//==================================Fix======================================//
void                Rst_FixNet          ( Route * route, Net * net );
void                Rst_FixEdge         ( Route * route, EDGE edge );

//==================================Edge Features============================//
int                 Rst_EdgeOverflow    ( Route * route, EDGE edge );
int                 Rst_EdgesOverflow   ( Route * route, EDGES * edge );
int                 Rst_EdgeWeight      ( Route * route, EDGE edge );
int                 Rst_EdgesWeight     ( Route * route, EDGES * edge );

//==================================Util Features============================//
int                 Rst_UpdateUtil      ( Route * route, EDGES * edges );
int                 Rst_ReleaseUtil     ( Route * route, EDGES * edges );
int                 Rst_UpdateUtil      ( Route * route, EDGE edge );
int                 Rst_ReleaseUtil     ( Route * route, EDGE edge );
void                Rst_CleanUtil       ( Route * route );

//==================================Weight Features==========================//
void                Rst_InitWeight      ( Route * route );
void                Rst_UpdateWeight    ( Route * route, EDGES * edges );

//==================================Print Stats==============================//
void                Rst_PrintStat       ( Route * route );
void                Rst_PrintNetResult  ( Route * route, Net * net );
void                Rst_PrintHeader     ( Route * route );


//==================================Others===================================//
bool                Rst_PointIsValid    ( Route * route, Point point );
int                 Rst_GetOverflow     ( Route * route );
#endif