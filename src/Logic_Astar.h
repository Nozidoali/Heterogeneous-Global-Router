#ifndef LOGIC_ASTAR_H
#define LOGIC_ASTAR_H

#include "Basic_RoutingInst.h"

/**
 * Astar_Man:
 * 
 * The manager of local shortest path problem
 * 1. Store the scope of local problem.
 * 2. Store the edge information
 */
class Astar_Man {

public:
    RoutingInst * rst;
    Point start, end;
    int x_lower, x_upper, y_lower, y_upper, area;
    int overflow, wirelength;
    bool * mem_visited, * mem_map_x, * mem_map_y;
    int * mem_distance;
    DIRECT * mem_dir;

    Astar_Man (){}
    Astar_Man ( RoutingInst * _rst);
    Astar_Man ( RoutingInst * _rst, Point start, Point end , double Flex = 0 );
    ~Astar_Man();
// =============================Properties================================== //
    bool IsValid    ( Point a );
    bool isVisited  ( Point a );
    bool IsUsed     ( Point a, Point b );
    bool IsFull();

// =============================Search Related============================== //
    int  ToIndex    ( Point a );
    void visit      ( Point a );
    bool go         ( Point a, DIRECT dir, int cost );
    void retrace    ( Point p );
    void Update     ( Segment * segment );

// =============================Grid Releted================================ //
    void AddSegment ( Segment * Segment );
    void AddEdge    ( Point p, DIRECT dir );
    void AddFragment( Point start, Point end );
    int  getOverflow ();
    int  getWireLength ();
    void SaveUtil   ();
    void RemoveUtil   ();
    
};

#endif