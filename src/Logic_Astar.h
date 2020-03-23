#ifndef LOGIC_ASTAR_H
#define LOGIC_ASTAR_H

#include "Logic_RoutingInst.h"

// the local area to search
struct Astar_Man {
    RoutingInst * rst;
    Point start, end;
    int x_lower, x_upper, y_lower, y_upper, area;
    int overflow, wirelength;
    bool * mem_visited;
    int * mem_distance, * mem_dir;
    bool * mem_map_x, * mem_map_y;
    Astar_Man (){}
    Astar_Man ( RoutingInst * _rst);
    Astar_Man ( RoutingInst * _rst, Point start, Point end , double Flex = 1 );
    ~Astar_Man() {
        delete [] mem_dir;
        delete [] mem_visited;
        delete [] mem_distance;
        delete [] mem_map_x;
        delete [] mem_map_y;
    }
    bool IsValid( Point a ) {
        return a.x>=x_lower && a.x<x_upper && a.y>=y_lower && a.y<y_upper;
    }
    // Find the index coordinated with respect to the region
    int ToIndex( Point a ) {
        return (a.x-x_lower) + (a.y-y_lower)*(x_upper-x_lower);
    }
    void visit( Point a ) {
        mem_visited[ToIndex(a)] = true;
    }
    bool isVisited( Point a ) {
        return mem_visited[ToIndex(a)];
    }
    bool go( Point a, int dir, int cost ) {
        bool ret = cost < mem_dir[ToIndex(a)] || mem_dir[ToIndex(a)] == -1;
        if (ret) {
            mem_dir[ToIndex(a)] = dir;
            mem_distance[ToIndex(a)] = cost;
        }
        return ret;
    }

    // Retrace
    void retrace( Point p );
    void Update( Segment * segment );
    bool IsUsed( Point a, Point b );
    bool IsFull();
};

#endif