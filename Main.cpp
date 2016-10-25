#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <random>


#include "vasicekMain.cpp"
#include "deMain.cpp"

int main()
{
  // In the main we go through each time-serie and get the optimized model
  // parameters as well as the final yield curve for each time-serie

  // Define an array for Model Parameters e.g. alpha, beta, sigma
  std::array<double , 12> alphaArray;
  std::array<double , 12> betaArray;
  std::array<double , 12> sigmaArray;

  // Maturity [0.25, 1, 3, 5, 7, 10, 15, 20, 30]
  const int maturityCount = 9;

  // Jan 2015 bis Dec. 2015
  const int timeSeriesCount = 12;
  // this is the value that stores the final modelYields
  std::array< std::array< double, maturityCount>, timeSeriesCount> modelYields;

  // Call the Differential Evolution Function
  // for each time-serie
  // for(int i = 0; i < timeSeriesCount; i++)
  // {
  //   deMain()
  // }



}
