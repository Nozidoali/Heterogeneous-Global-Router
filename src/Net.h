#ifndef NET_H
#define NET_H

#include "Task.h"

typedef pair<Point,Point> SEGMENT;
typedef vector<SEGMENT> SEGMENTS;
/**
 * A structure to represent nets
 */
struct Net
{

    int overflow;
    int id ; 		            /* ID of the net */
    int numPins;                /* number of pins */
    Point * pins; 		        /* array of pins (or terminals) of the net. */
    EDGES * edges;              /* solution of the net */
    SEGMENTS segments;
    
};

Tasks *         Net_CreateTask      ( Net * net );
void            Net_CollectResult   ( Net * net, Tasks * tasks );
int             Net_GetWirelength   ( Net * net );
bool            Net_HasResult       ( Net * net );
void            Net_CleanResult     ( Net * net );
void            Net_Free            ( Net * net );

#endif