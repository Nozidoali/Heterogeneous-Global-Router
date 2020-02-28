cd ..
cmake .
make
./ROUTER.exe benchmarks/adaptec1.gr benchmarks/adaptec1.out
./ROUTER.exe benchmarks/adaptec2.gr benchmarks/adaptec2.out
./ROUTER.exe benchmarks/adaptec3.gr benchmarks/adaptec3.out

./556_eval.exe benchmarks/adaptec1.gr benchmarks/adaptec1.out 1
./556_eval.exe benchmarks/adaptec2.gr benchmarks/adaptec2.out 1
./556_eval.exe benchmarks/adaptec3.gr benchmarks/adaptec3.out 1