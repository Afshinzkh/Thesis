# Calibrating Short-Rate Models Using Differential Evolution On A GPU


This is my Master Thesis that I have done in cooperataion with risklab gmbh - Allianz at Munich. 
Finished in March 2017.

## Short Description

We have two different Financial Models, the Vasicek and the CIR model. In this work I have used the differential evolution algorithm to calibrate these two models. I wrote three different Models all of them available in my git. 
* [Serial Version written with C++ (This Git)](https://github.com/Afshinzkh/Thesis) 
* [Parallel on CPU using OpenMP](https://github.com/Afshinzkh/DEonCPUParallel)
* [Parallel on GPU using CUDA](https://github.com/Afshinzkh/DifferentialEvolutionGPU)



# Dependencies
* g++ 4.8 or higher

## How to Compile and run
* Compile with: 
```
g++ -std=c++11 -o solver Main.cpp Vasicek.cpp CIR.cpp
```
* run     with:		
```
./solver data12.csv vasicek (or cir)
```
## Complete Thesis Paper
Can be found in the folder Documentation
