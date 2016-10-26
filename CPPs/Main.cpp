#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <random>


#include "../Headers/vasicekMain.h"
#include "../Headers/deMain.h"
using namespace Calibration;

int main()
{
  // In the main we go through each time-serie and get the optimized model
  // parameters as well as the final yield curve for each time-serie

  /****************************************************************************/
	/******************** STEP 1 : Initialize variables *************************/
	/****************************************************************************/
  // Define an array for Model Parameters e.g. alpha, beta, sigma
  // for now the length is 12 since we want to get the values for 12 months
  std::array<double , 12> alphaArray;
  std::array<double , 12> betaArray;
  std::array<double , 12> sigmaArray;

  // Maturity [0.25, 1, 3, 5, 7, 10, 15, 20, 30]
  const int maturityCount = 9;
  // Jan 2015 bis Dec. 2015
  const int timeSeriesCount = 12;

  // this is the value that stores the final modelYields
  std::array< std::array< double, maturityCount>, timeSeriesCount> modelYields;

  /****************************************************************************/
	/******************** STEP 1 : Read the Data ********************************/
	/****************************************************************************/
  // read the data from file e.g. for 12 month
  // it is a matrix named mrktData and has the size of
  // sereisCount * maturityCount with seriesCount as the number of month
  // and maturityCount as the number of maturities. e.g mrktData [12 * 9]
  // we also have an array tau [maturityCount] as the Time to maturity

  std::array<double,9> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
  // define the data coloumn 1 size bigger so it doesn't go ham while reading
  // TODO: think of reading the data without knowing the row size
  std::array<std::array<double,9>, 12> mrktData;

  std::ifstream dataFile("../Data/Data.csv");
  int row = 0;
  int col = 0;
  if(dataFile.is_open())
  {
    // std::cout << "Data File is Opened" << std::endl;

    std::string aLine;
    while(getline(dataFile, aLine))
    {
      std::istringstream ss(aLine);
      std::string num;
      while(ss >> num)
      {
        mrktData[row][col] = std::stod(num.c_str());
        col++;
      }
      row++;
      col = 0;
    }
  }
  dataFile.close();
  // std::cout << "reached end of file" << std::endl;

  // read the first row of mrktData wich would be for the first month e.g. Jan.2015
  // and put this into an array called crrntMonthMrktData with size of [1 * maturityCount]
  std::array<double,9> crrntMonthMrktData;

  DE d;
  std::array <std::string, 12> monthNames = {
    "Jan.2015", "Feb.2015", "Mar.2015", "Apr.2015", "May.2015",
    "Jun.2015", "Jul.2015", "Aug.2015", "Sep.2015", "Oct.2015",
    "Nov.2015", "Dec.2015"
  };
  // Call the Differential Evolution Function
  // for each time-serie
  for(int i = 0; i < timeSeriesCount; i++)
  {
    crrntMonthMrktData = mrktData[timeSeriesCount-1-i];
    std::cout << "=============================" << std::endl;
    std::cout << "Running DE for :" << monthNames[i] << std::endl;
    d.runDE(crrntMonthMrktData);
  }


  return 0;
}
