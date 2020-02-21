// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.


#ifndef ECE556_H
#define ECE556_H

#include <stdio.h>

 /**
  * A structure to represent a 2D Point. 
  */
 typedef struct
 {
   int x ; /* x coordinate ( >=0 in the routing grid)*/
   int y ; /* y coordinate ( >=0 in the routing grid)*/

 } Point ;


  /**
  * A structure to represent a segment
  */
 typedef struct
 {
   Point p1 ; 	/* start point of a segment */
   Point p2 ; 	/* end point of a segment */
   
   int numEdges ; 	/* number of edges in the segment*/
   int *edges ;  	/* array of edges representing the segment*/
   
 } Segment ;
 
 
  /**
  * A structure to represent a route
  */
  typedef struct
  {
    int numSegs ;  	/* number of segments in a route*/
    Segment *segments ;  /* an array of segments (note, a segment may be flat, L-shaped or any other shape, based on your preference */

  } route ;
 
 
  /**
  * A structure to represent nets
  */
  typedef struct
  {

   int id ; 		/* ID of the net */
   int numPins ; 		/* number of pins (or terminals) of the net */
   Point *pins ; 		/* array of pins (or terminals) of the net. */
   route nroute ;		/* stored route for the net. */

  } Net ;
  
  /**
  * A structure to represent the routing instance
  */
  typedef struct
  {
   int gx ;		/* x dimension of the global routing grid */
   int gy ;		/* y dimension of the global routing grid */
   
   int cap ;
   
   int numNets ;	/* number of nets */
   Net *nets ;		/* array of nets */
   
   int numEdges ; 	/* number of edges of the grid */
   int *edgeCaps; 	/* array of the actual edge capacities after considering for blockages */
   int *edgeUtils;	/* array of edge utilizations */  
   
  } RoutingInst ;

#endif // ECE556_H

