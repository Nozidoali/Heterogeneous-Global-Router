#include "Task.h"

Task Tsk_Init( Point start, Point end ) {
    Task task;
    task.start = start;
    task.end   = end;
    task.edges = NULL;
    task.difficulty = 0;
    return task;
}

bool Tsk_HasResult( const Task & task ) {
    return ( task.edges != NULL );
}

EDGES * Tsk_GetResult( const Task & task ) {
    return task.edges;
}

void Tsk_Append( Task & task, EDGE edge ) {
    assert( task.edges != NULL );
    task.edges->insert( edge );
}

int Tsk_GetWirelength( const Task & task ) {
    return (int)task.edges->size();
}

void Tsk_CleanResult( Task & task ) {
    if ( task.edges == NULL )
        return;
    task.edges->clear();
    delete task.edges; task.edges = NULL;
}

void Tsk_Free( Tasks * pTasks ) {
    if ( pTasks == NULL )
        return;
    for ( auto & task : *pTasks ) {
        Tsk_CleanResult( task );
    }
    pTasks->clear();
    delete pTasks; pTasks = NULL;
}

bool Tsk_PointIsValid( const Task & task, Point point ) {
    return ( point >= ( (task.start -= task.end) - Point(task.difficulty, task.difficulty)) ) 
        && ( point <= ( (task.start += task.end) + Point(task.difficulty, task.difficulty)) );
}

void Tsk_SetDifficulty ( Task & task, int difficulty ) {
    task.difficulty = difficulty;
}

EDGES * Tsk_CollectResult ( Tasks * pTasks ) {

    EDGES * edges = new EDGES;
    for ( auto & task : *pTasks ) {
        assert( Tsk_HasResult( task ) );
        EDGES edges_copy = *( Tsk_GetResult( task ) );
        for ( auto & edge : *edges ) {
            edges->insert( edge );
        }
    }
    return edges;

}

void Edg_Free ( EDGES * edges ) {
    if ( edges == NULL )
        return;
    edges->clear();
    delete edges; edges = NULL;
}