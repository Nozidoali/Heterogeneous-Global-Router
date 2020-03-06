#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>

using namespace std;

/**
 * A structure to represent a 2D Point. 
 */
class Point
{
public:
    int x ; /* x coordinate ( >=0 in the routing grid)*/
    int y ; /* y coordinate ( >=0 in the routing grid)*/
    Point();
    Point(int _x, int _y);
    const string toString();
    bool operator == (const Point p) const;
};

/**
 * A structure to represent a segment
 */
class Segment
{
public:
    Point p1 ; 	/* start point of a segment */
    Point p2 ; 	/* end point of a segment */

    int numEdges ; 	/* number of edges in the segment*/
    int *edges ;  	/* array of edges representing the segment*/

    int numFragment; /* number of fragments in the segment*/
    vector<pair<Point, Point> > fragments; /* array of fragments in the segment*/
};


/**
 * A structure to represent a route
 */
class Route
{
public:
    int numSegs ;  	/* number of segments in a route*/
    vector<Segment *> segments ;  /* an array of segments (note, a segment may be flat, L-shaped or any other shape, based on your preference */

};


/**
 * A structure to represent nets
 */
class Net
{
public:
    int id ; 		/* ID of the net */
    int numPins ; 		/* number of pins (or terminals) of the net */
    Point *pins ; 		/* array of pins (or terminals) of the net. */
    Route nroute ;		/* stored route for the net. */

};

/**
 * A structure to represent the routing instance
 */
class RoutingInst
{
public:
    int gx ;		/* x dimension of the global routing grid */
    int gy ;		/* y dimension of the global routing grid */

    int cap ;

    int numNets ;	/* number of nets */
    Net *nets ;		/* array of nets */

    int numEdges ; 	/* number of edges of the grid */
    int *edgeCaps; 	/* array of the actual edge capacities after considering for blockages */
    int *edgeUtils;	/* array of edge utilizations */  

};

/** 
 *  class of Hannan Grid which extrace the x and y coordinates of each pin
 *  Descrete the grid.
 */
class hGrid {
private:

    int * mem_x;
    int * mem_y;
    map<int, int> mem_ix;
    map<int, int> mem_iy;
    Point * pins;
public:
    int gx;
    int gy;
    hGrid(RoutingInst * rst, Net * net);
    ~hGrid();
    Point toIndex(Point p);
    Point toPoint(Point index);
};

/**
 * Global Definitions
 */
static const int SUCCESS = 1;
static const int FAILED = 0;

/**
 * Direction of the edge
 */
enum DIRECT {
    VERTICAL,
    HORIZONTAL
};

/**
 * Functions ./base/base.h
 */
extern pair<Point, Point>   toPoint         (int edgeNum, int gx, int gy);
extern int                  toEdge          (int x1, int x2, int y1, int y2, int gx, int gy);
extern int                  toEdge          (Point * a, Point * b, int gx, int gy);
extern int                  release         (RoutingInst *rst);

/**
 * Functions ./util/util.h
 */
extern int          distance        (Point start, Point end);
extern int          distance        (pair<Point, Point> p);
extern Segment *    lSegment        (RoutingInst * rst, Point start, Point end);

/**
 * Function ./util/solver.h
 */
extern int          solveNet        (RoutingInst * rst, Net * net);
extern int          solveRouting    (RoutingInst *rst);

/**
 * Functions ./util/fileio.h
 */
extern int          readBenchmark   (const char *fileName, RoutingInst *rst);
extern int          writeOutput     (const char *outRouteFile, RoutingInst *rst);

/**
 * Functions ./util/RMST.h
 */
extern void *       startRMST       (RoutingInst *_rst, Net * _net);
extern void         endRMST         (void * rmst);
extern int          solveRMST       (void * rmst);

#endif