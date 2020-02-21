// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.


int main(int argc, char **argv)
{

 	if(argc!=3){
 		printf("Usage : ./ROUTE.exe <input_benchmark_name> <output_file_name> \n");
 		return 1;
 	}

 	int status;
	char *inputFileName = argv[1];
 	char *outputFileName = argv[2];

 	/// create a new routing instance
 	routingInst *rst = new routingInst;
	
 	/// read benchmark
 	status = readBenchmark(inputFileName, rst);
 	if(status==0){
 		printf("ERROR: reading input file \n");
 		return 1;
 	}
	
 	/// run actual routing
 	status = solveRouting(rst);
 	if(status==0){
 		printf("ERROR: running routing \n");
 		release(rst);
 		return 1;
 	}
	
 	/// write the result
 	status = writeOutput(outputFileName, rst);
 	if(status==0){
 		printf("ERROR: writing the result \n");
 		release(rst);
 		return 1;
 	}

 	release(rst);
 	printf("\nDONE!\n");	
 	return 0;
}
