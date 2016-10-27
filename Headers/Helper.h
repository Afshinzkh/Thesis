// Helper functions Are declared and defined in this hedear
// functions are:
//          readData: Reads the Data from file

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <array>
#include <cstring>

namespace Calibration{

  template <size_t M, size_t N>
  void readData(std::string dataFileName,
      std::array< std::array <double, N>, M> &dataArray)
  {
    dataFileName = "../Data/" + dataFileName;
    std::cout << "file: " << dataFileName << std::endl;

    std::ifstream dataFile(dataFileName);
    int row = 0;
    int col = 0;
    if(dataFile.is_open())
    {
      std::string aLine;
      while(getline(dataFile, aLine))
      {
        std::istringstream ss(aLine);
        std::string num;
        while(ss >> num)
        {
          dataArray[row][col] = std::stod(num.c_str());
          col++;
        }
        row++;
        col = 0;
      }
    }
    dataFile.close();
  }


}
