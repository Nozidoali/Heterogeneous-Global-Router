#ifndef NET_H
#define NET_H

#include "Basic_Point.h"
#include "Basic_Route.h"

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

#endif