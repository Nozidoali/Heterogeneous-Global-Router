#ifndef NET_H
#define NET_H

#include "Task.h"

typedef pair<Point,Point> SEGMENT;
typedef vector<EDGES> SEGMENTS;
/**
 * A structure to represent nets
 */
struct Net
{

    int cost;
    int overflow;
    int id ; 		            /* ID of the net */
    int numPins;                /* number of pins */
    vector<Point> pins; 		/* array of pins (or terminals) of the net. */
    EDGES * edges;              /* solution of the net */
    Point upper, lower;
    
};

//==================================Task Management==========================//
Tasks *         Net_CreateTaskMST   ( Net * net );
Tasks *         Net_CreateTask      ( Net * net );
void            Net_CollectResult   ( Net * net, Tasks * tasks );

//==================================Edge Management==========================//
void            Net_RemoveEdge      ( Net * net, EDGES::iterator edge );
void            Net_AddEdge         ( Net * net, EDGE edge );
void            Net_AddEdges        ( Net * net, EDGES * edges );

//==================================Features=================================//
int             Net_GetWirelength   ( const Net * net );
int             Net_GetOverflow     ( const Net * net );
int             Net_GetArea         ( const Net * net );
bool            Net_HasResult       ( const Net * net );

//==================================Memory Control===========================//
void            Net_CleanResult     ( Net * net );
void            Net_Free            ( Net * net );

//==================================Printing=================================//
void            Net_PrintResult     ( const Net * net );

#endif