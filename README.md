# Router_Project

> Implement of a CAD tool of global routing.  

<hr>
<center> Copyright@why </center>

## Platform
Please build this program on a 64-bit UNIX like system with library `pthread`. Dependency:
```bash
    sudo apt-get install make cmake gcc g++
```

## Compile
After updating the source file, you need to update Makefile by cmakelist first.
```
    cmake .
    make
```

Please note that you need `CMake version 2.7` and `C++14 feature`.

## Run
```
    Usage : ./ROUTE.exe <input_benchmark_name> <output_file_name>
```