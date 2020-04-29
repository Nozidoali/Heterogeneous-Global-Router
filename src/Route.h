#ifndef ROUTE_H
#define ROUTE_H

#include "Net.h"
#include "Logic_Timer.h"

#define OF_MAX 2147483647
#define AUTO   -1

enum MODE {
    EASY,
    MEDIUM,
    DIFFICULT,
    HARD
};

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
    int *edgeReserveCaps;	/* array of edge utilizations */  
    int *edgeWeights;

    bool isDecomposition;
    bool isOrdered;

    int overflow;

//==================================Parameters===============================//
    int Initial_Max_Difficulty;
    int Initial_Max_TaskScale;
    int Reroute_Max_TaskScale;
    int Reroute_Max_Complexity; // Scale * difficulty
    int Reroute_Max_Difficulty;

//==================================Estimation===============================//
    double  *utilExp;

};

//==================================Memory Control===========================//
Route *             Rst_Start       ( bool isDecomposition, bool isOrdered );
void                Rst_Stop        ( Route * route );

//==================================Initialization===========================//
void                Rst_Init        ( Route * route );
void                Rst_NetInitTasks( Route * route, Net * net );

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
int                 Rst_RerouteTask     ( Route * route, Task & net, int limit );
int                 Rst_ReduceNet       ( Route * route, Net * net );

//==================================Fix======================================//
void                Rst_FixNet          ( Route * route, Net * net );
void                Rst_FixEdge         ( Route * route, EDGE edge );

//==================================Edge Features============================//
int                 Rst_EdgeWeight      ( Route * route, EDGE edge );
int                 Rst_EdgeOverflow    ( Route * route, EDGE edge );
int                 Rst_EdgesWeight     ( Route * route, EDGES * edge );
int                 Rst_EdgesOverflow   ( Route * route, EDGES * edge );
int                 Rst_GetOverflow     ( Route * route );

//==================================Util Features============================//
void                Rst_CleanUtil       ( Route * route );
int                 Rst_UpdateUtil      ( Route * route, EDGES * edges );
int                 Rst_UpdateUtil      ( Route * route, EDGE edge );
int                 Rst_ReleaseUtil     ( Route * route, EDGES * edges );
int                 Rst_ReleaseUtil     ( Route * route, EDGE edge );

//==================================Weight Features==========================//
void                Rst_InitWeight      ( Route * route );
void                Rst_UpdateWeight    ( Route * route, EDGES * edges );

//==================================Print Stats==============================//
void                Rst_PrintStat       ( Route * route );
void                Rst_PrintNetResult  ( Route * route, Net * net );
void                Rst_PrintHeader     ( Route * route );
void                Rst_PrintWireDist   ( Route * route );
void                Rst_PrintCapsDist   ( Route * route );
void                Rst_PrintPinsDist   ( Route * route );

//==================================Others===================================//
bool                Rst_PointIsValid    ( Route * route, Point point );

//==================================Capacity=================================//
void                Rst_ReserveCap      ( Route * route );
void                Rst_ReleaseCap      ( Route * route );

//==================================Mode Selection===========================//
MODE                Rst_GetMode         ( Route * route );
void                Rst_SetMode         ( Route * route, MODE mode );

//==================================Estimation===============================//
void                Rst_InitEstimation  ( Route * route );
bool                Rst_HasEstimation   ( Route * route );
void                Rst_ClearEstimation ( Route * route );
double              Rst_GetEstimation   ( Route * route, Point start, Point end );
double              Rst_GetOfEstimation ( Route * route, Point start, Point end );

#endif