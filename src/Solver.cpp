#include "Solver.h"

RoutingInst * Solver :: GetInst() {
    return rst;
}

const ifstream operator >> ( ifstream finput, Solver * solver ) {
    string command;
    RoutingInst * rst = solver->GetInst();
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
                throw(exception());
            }
        }
        else {
            continue;
        }
    }
    return finput;
} 

ofstream operator << ( ofstream foutput, Solver * solver ) {
    RoutingInst * rst = solver->GetInst();
    for(int i=0;i<rst->numNets;i++) {
        // print the node ID in the first line
        foutput << "n" << rst->nets[i].id << endl;
        for(int j=0;j<rst->nets[i].nroute.numSegs;j++) {
            Segment * segment = rst->nets[i].nroute.segments[j];
            for(int k=0;k<segment->numFragment;k++) {
                // print the edge in the correct format
                foutput << segment->fragments[k].first.toString() << "-" << segment->fragments[k].second.toString() << endl;
            }
        }
        foutput << "!" << endl;
    }
    return foutput;
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
        // allow comment
        if (command == "#") {
            finput >> command;
            while (command != "#") {
                finput >> command;
            }
            continue;
        }
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
            continue;
        }
    }
    finput.close();
    return SUCCESS;
}

int solveRouting(RoutingInst *rst){
#ifdef SOLVER_RMST
    // for each net in the routing instance
    for(int i=0;i<rst->numNets;i++) {
        Net * net = rst->nets + i;
        void * rmst = startRMST(rst, net);
        solveRMST(rmst);
        endRMST(rmst);
    }
    return SUCCESS;
#endif
#ifdef SOLVER_WEIGHTED
    Weighted_Solver solver( rst );
    solver.Solve();
    return SUCCESS;
#endif
}

int writeOutput(const char *outRouteFile, RoutingInst *rst) {
    ofstream foutput(outRouteFile);
    for(int i=0;i<rst->numNets;i++) {
        // print the node ID in the first line
        foutput << "n" << rst->nets[i].id << endl;
        for(int j=0;j<rst->nets[i].nroute.numSegs;j++) {
            Segment * segment = rst->nets[i].nroute.segments[j];
            for(int k=0;k<segment->numFragment;k++) {
                // print the edge in the correct format
                foutput << segment->fragments[k].first.toString() << "-" << segment->fragments[k].second.toString() << endl;
            }
        }
        foutput << "!" << endl;
    }
    return SUCCESS;
}

int release(RoutingInst *rst) {
    delete [] rst->edgeCaps;
    delete [] rst->edgeUtils;
    for (int i=0;i<rst->numNets;i++) {
        for(int j=0;j<rst->nets[i].nroute.numSegs;j++) {
            rst->nets[i].nroute.segments[j]->fragments.clear();
            delete rst->nets[i].nroute.segments[j];
        }
        rst->nets[i].nroute.segments.clear();
        delete [] rst->nets[i].pins;
    }
    delete [] rst->nets;
    delete rst;
    return SUCCESS;
}