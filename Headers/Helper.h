// Helper functions Are declared and defined in this hedear
// functions are:
//          readData: Reads the Data from file
//          writeData: write the data to results.csv

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <array>
#include <cstring>

namespace Calibration{

  // e.g. M = 12 , N = 9
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

  template <size_t M, size_t N>
  void writeData(std::array< std::array <double, N>, M> &mdlArray,
          std::array< std::array <double, N>, M> &mrktArray,
          std::array< double, M> &alphaArray, std::array< double, M> &betaArray,
          std::array< double, M> &sigmaArray, std::array< double, M> &errorArray,
          std::array< double, M> &iterArray, std::array< double, M> &timeArray)
  {

    std::string dataFileName = "../Data/results.csv";
    std::ofstream dataFile (dataFileName);


    for (size_t i = 0; i < M; i++) {
        dataFile << "Date;Alpha;Beta;Sigma;Error;Time;Iterations;\
  ModR0.25;ModR1;ModR3;ModR5;ModR7;ModR10;ModR15;ModR20;ModR30;\
  MarR0.25;MarR1;MarR3;MarR5;MarR7;MarR10;MarR15;MarR20;MarR30\n";

        dataFile << i+1 << ";";
        dataFile << alphaArray[i] << ";";
        dataFile << betaArray[i] << ";";
        dataFile << sigmaArray[i] << ";";
        dataFile << errorArray[i] << ";";
        dataFile << timeArray[i] << ";";
        dataFile << iterArray[i] << ";";
        for (size_t j = 0; j < N; j++)
          dataFile << mdlArray[i][j] << ";";
        for (size_t j = 0; j < N; j++)
            dataFile << mrktArray[11-i][j] << ";";
        dataFile << "\n ";
        dataFile << "\n ";

        // dataFile << " Data for time-serie: " << i+1 << std::endl;
        // dataFile << "Model\n ";
        // dataFile << "0.25;1;3;5;7;10;15;20;30;\n ";
        // for (size_t j = 0; j < N; j++) {
        //   dataFile << mdlArray[i][j] << ";";}
        //
        // dataFile << "\n ";
        //
        //
        //
        // dataFile << "Market\n ";
        // dataFile << "0.25;1;3;5;7;10;15;20;30;\n ";
        // for (size_t j = 0; j < N; j++) {
        //   dataFile << mrktArray[11-i][j] << ";";}
        //
        // dataFile << "\n ";
        //
        // dataFile << "Final Parameters:\n";
        // dataFile << "Alpha;" << alphaArray[i] << ";;";
        // dataFile << "Beta;" << betaArray[i] << ";;";
        // dataFile << "Sigma;" << sigmaArray[i] << ";;";
        //
        // dataFile << "\n ";
        //
        // dataFile << "Iterations;" << iterArray[i] << ";;";
        // dataFile << "Error;" << errorArray[i] << ";;";
        // dataFile << "Time;" << timeArray[i] << ";;";
        //
        // dataFile << "\n ";
        // dataFile << "\n ";

        }
    dataFile.close();
  }

  template <size_t M, size_t N>
  void writeRisklabData(std::array< std::array <double, N>, M> &mdlArray,
          std::array< std::array <double, N>, M> &mrktArray,
          std::array< double, M> &alphaSArray, std::array< double, M> &betaSArray,
          std::array< double, M> &sigmaSArray,
          std::array< double, M> &alphaYArray, std::array< double, M> &betaYArray,
          std::array< double, M> &sigmaYArray,
          std::array< double, M> &alphaZArray, std::array< double, M> &betaZArray,
          std::array< double, M> &sigmaZArray,
          std::array< double, M> &weightYArray,std::array< double, M> &weightZArray,
           std::array< double, M> &errorArray,
          std::array< double, M> &iterArray, std::array< double, M> &timeArray)
  {

    std::string dataFileName = "../Data/results.csv";
    std::ofstream dataFile (dataFileName);


    for (size_t i = 0; i < M; i++) {
        dataFile << "Date;AlphaS;BetaS;SigmaS;AlphaY;BetaY;SigmaY;AlphaZ;BetaZ;SigmaZ;\
        weightY;weightZ;Error;Time;Iterations;\
  ModR1;ModR3;ModR5;ModR7;ModR10;ModR15;ModR20;ModR30;\
  MarR1;MarR3;MarR5;MarR7;MarR10;MarR15;MarR20;MarR30\n";

        dataFile << i+1 << ";";
        dataFile << alphaSArray[i] << ";";
        dataFile << betaSArray[i] << ";";
        dataFile << sigmaSArray[i] << ";";
        dataFile << alphaYArray[i] << ";";
        dataFile << betaYArray[i] << ";";
        dataFile << sigmaYArray[i] << ";";
        dataFile << alphaZArray[i] << ";";
        dataFile << betaZArray[i] << ";";
        dataFile << sigmaZArray[i] << ";";
        dataFile << weightYArray[i] << ";";
        dataFile << weightZArray[i] << ";";
        dataFile << errorArray[i] << ";";
        dataFile << timeArray[i] << ";";
        dataFile << iterArray[i] << ";";
        for (size_t j = 0; j < N; j++)
          dataFile << mdlArray[i][j] << ";";
        for (size_t j = 0; j < N; j++)
            dataFile << mrktArray[11-i][j] << ";";
        dataFile << "\n ";
        dataFile << "\n ";

        }
    dataFile.close();
  }



}
