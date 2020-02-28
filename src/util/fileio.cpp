#include "fileio.h"

#include <fstream>
#include <cstring>
#include <cassert>
using namespace std;

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
            rst->numEdges = 2*rst->gx*rst->gy - rst->gx - rst->gy;
            rst->edgeCaps = new int[rst->gx*rst->gy*2];
            rst->edgeUtils = new int[rst->gx*rst->gy*2];
        }
        else if (command == "capacity") {
            // read the default capacity of each edge
            finput >> rst->cap;
            for (int i=0;i<rst->gx*rst->gy*2;i++) {
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
                for (int i=0; i<rst->numNets; i++) {
                    rst->nets[i].id = i;
                    finput >> command;
                    // read the number of pins in the net
                    finput >> rst->nets[i].numPins;
                    rst->nets[i].pins = new Point[rst->nets[i].numPins];
                    for (int j=0; j<rst->nets[i].numPins; j++) {
                        // read the x,y coordinates of the pin
                        finput >> rst->nets[i].pins[j].x >> rst->nets[i].pins[j].y;
                    }
                }
                // read the blockages
                int blockages;
                finput >> blockages;
                for (int i=0, x1, y1, x2, y2, c;i<blockages;i++) {
                    finput >> x1 >> y1 >> x2 >> y2 >> c;
                    int index = toEdge(x1,y1,x2,y2,rst->gx,rst->gy);
                    assert(index>=0 && index<rst->gx*rst->gy*2);
                    rst->edgeCaps[index] = c;
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

int writeOutput(const char *outRouteFile, RoutingInst *rst) {
    ofstream foutput(outRouteFile);
    for(int i=0;i<rst->numNets;i++) {
        // print the node ID in the first line
        foutput << "n" << rst->nets[i].id << endl;
        for(int j=0;j<rst->nets[i].nroute.numSegs;j++) {
            Segment * segment = rst->nets[i].nroute.segments[j];
            for(int k=0;k<segment->numEdges;k++) {
                int edgeIndex = segment->edges[k];
                pair<Point, Point> edge = toPoint(edgeIndex, rst->gx, rst->gy);
                // print the edge in the correct format
                foutput << edge.first.toString() << "-" << edge.second.toString() << endl;
            }
        }
        foutput << "!" << endl;
    }
    return SUCCESS;
}
