#ifndef EDGE_H
#define EDGE_H

#include "Point.h"

typedef int EDGE;
typedef vector<EDGE> EDGES;

struct Task 
{

    int difficulty;
    Point start, end;
    EDGES * edges;

};

typedef vector<Task> Tasks;

Task            Tsk_Init            ( Point start, Point end );
bool            Tsk_HasResult       ( const Task & task );
const EDGES &   Tsk_GetResult       ( const Task & task );
int             Tsk_GetWirelength   ( const Task & task );
void            Tsk_Append          ( Task & task, EDGE edge );
void            Tsk_CleanResult     ( Task & task );
void            Tsk_Free            ( Tasks * pTasks );
bool            Tsk_PointIsValid    ( const Task & task, Point point );
void            Tsk_SetDifficulty   ( Task & task, int difficulty );
#endif