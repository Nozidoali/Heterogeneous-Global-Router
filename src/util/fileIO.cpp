#include "../base/base.h"

#define SUCCESS 1
#define FAILED 0

#include <fstream>
#include <cstring>
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
            rst->numEdges = 2*rst->gx*rst->gy-rst->gx-rst->gy;
            for
        }
        else if (command == "capacity") {
            // read the default capacity of each edge
            finput >> rst->cap;
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
                finput >> 
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