# test the util functions
cd ../src
# check the output
./main ../benchmarks/input ../benchmarks/output
# check the memory leak
valgrind ./main ../benchmarks/input ../benchmarks/output