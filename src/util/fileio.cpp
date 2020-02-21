#include "util.h"

#define SUCCESS 1
#define FAILED 0

#include <fstream>
#include <cstring>
using namespace std;

/**
 * Return the index of edge between two given point.
 */
int toEdge(int x1, int x2, int y1, int y2, int gx, int gy) {
    int x = (x1+x2)>>1;
    int y = (y1+y2)>>1;
    if (x1==x2) {
        x+y*gx;
    }
    else if (y1==y2) {
        x*y*(gx-1);
    }
    else {
        // no edge between two given edge
        return -1;
    }
}

int readBenchmark(const char *fileName, RoutingInst *rst) {
    ifstream finput(fileName);
    // file not opened correctly
    if (!finput.is_open()) {
        return FAILED;
    }
    string command;
    while (!finput.eof()) {
        finput >> command;
        if (command == "grid") {
            // read the x,y scale of the grid
            finput >> rst->gx >> rst->gy;
            rst->numEdges = 2*rst->gx*rst->gy-rst->gx-rst->gy;
            rst->edgeCaps = new int[rst->gx*rst->gy*2];
            rst->edgeUtils = new int[rst->gx*rst->gy*2];
        }
        else if (command == "capacity") {
            // read the default capacity of each edge
            finput >> rst->cap;
            for (unsigned int i=0;i<rst->gx*rst->gy*2;i++) {
                rst->edgeCaps[i] = rst->cap;
                rst->edgeUtils[i] = 0;
            }
        }
        else if (command == "num") {
            finput >> command;
            if (command == "net") {
                // read the numebr of nets
                finput >> rst->numNets;
                rst->nets = new Net[rst->numNets];
                for (unsigned int i=0; i<rst->numNets; i++) {
                    rst->nets[i].id = i;
                    // read the number of pins in the net
                    finput >> rst->nets[i].numPins;
                    rst->nets[i].pins = new Point[rst->nets[i].numPins];
                    for (unsigned int j=0; j<rst->nets[i].numPins; j++) {
                        // read the x,y coordinates of the pin
                        finput >> rst->nets[i].pins[j].x >> rst->nets[i].pins[j].y;
                    }
                }
                // read the blockages
                unsigned int blockages;
                finput >> blockages;
                for (unsigned int i=0, x1, y1, x2, y2, c;i<blockages;i++) {
                    finput >> x1 >> y1 >> x2 >> y2 >> c;
                    rst->edgeCaps[toEdge(x1,y1,x2,y2,rst->gx,rst->gy)] = c;
                }
            }
            else {
                finput.close();
                return FAILED;
            }
        }
        else {
            finput.close();
            return FAILED;
        }
    }
    finput.close();
    return SUCCESS;
}