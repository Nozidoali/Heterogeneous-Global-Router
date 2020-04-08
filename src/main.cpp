/// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.

#include "Route.h"

int main(int argc, char **argv)
{ 

 	if(argc!=3){
 		cout << "Usage : ./ROUTE.exe <input_benchmark_name> <output_file_name> \n";
 		return 1;
 	}

	const char *inputFileName  = argv[1];
 	const char *outputFileName = argv[2];

    try {
        Route * route = Rst_Start();
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
