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

## Performance

```
| Date  | Version |  TWL1   |  TOF1   |  TWL2   |  TOF2  |   TWL2   |  TOF2   |
| :---: | :-----: | :-----: | :-----: | :-----: | :----: | :------: | :-----: |
| 4 Mar |   1.0   | 5461421 | 1657741 | 5169015 | 466521 | 14946703 | 3143019 |
| 23Mar |   2.0   | 3545650 | 360411  | 3274452 | 56184  | 9638287  | 475280  |
```
