/// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.

#include "Route.h"
#include "string.h"

bool isDecomposition = false;
bool isOrdered = false;

int main(int argc, char **argv)
{ 

 	if(argc!=5){
 		cout << "Usage : ./ROUTE.exe -d=<> -n=<> <input_benchmark_name> <output_file_name> \n";
 		return 1;
 	}

    if ( strcmp(argv[1],"-d=1") == 0 ) {
        isDecomposition = true;
    }
    if ( strcmp(argv[2],"-n=1") == 0 ) {
        isOrdered = true;
    }

	const char *inputFileName  = argv[3];
 	const char *outputFileName = argv[4];

    try {
        Route * route = Rst_Start( isDecomposition, isOrdered );
        Rst_ReadFile    ( route, inputFileName );
        Rst_Solve       ( route );
        Rst_WriteFile   ( route, outputFileName );
        Rst_Stop        ( route );
    }
    catch( Exception ) {
        return 1;
    }
    
 	cout << "\nDONE!" << endl;	
 	return 0;
}
