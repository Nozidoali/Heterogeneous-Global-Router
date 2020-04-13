# Router_Project

> Implement of a CAD tool of global routing.  

<hr>
<center> Copyright@why </center>

## Platform
Please build this program on a 64-bit UNIX like system with library `pthread`. Dependency:
```bash
    sudo apt-get install make gcc g++
```

## Getting Started
After updating the source file, you need to update Makefile by cmakelist first.
```
    make
```

Please note that you need `C++14 feature`.

## Part.1

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

## Part.2

## Run
```
    Usage : ./ROUTE.exe -d=<Decomposition> -n=<Ordered> <input_benchmark_name> <output_file_name>
```

## Performance

***Adaptec1***:
```
| Parameter | Runtime |   TWL   |  TOF1  |
| :-------: | :-----: | :-----: | :----: |
| -d=1 -n=1 | 0:28.87 | 3471014 | 488451 |
| -d=1 -n=0 | 0:28.07 | 3471093 | 488286 |
| -d=0 -n=1 | 0:27.09 | 5410564 |1587515 |
| -d=0 -n=0 | 0:07.02 | 5461421 |1657741 |
```