#include "../Headers/Vasicek.h"
#include "../Headers/DE.h"
#include "../Headers/Helper.h"

using namespace Calibration;

// In the main we go through each time-serie and get the optimized model
// parameters as well as the final yield curve for each time-serie
int main(int argc, char* argv[])
{

  // Cheking the Arguments
  if( argc != 3){
    std::cout << "Error: Wrong number of Arguments" << std::endl;
    return -1;
  }

  std::string method = argv[2];
  std::cout << "Method to use: "<< method << std::endl;

  /****************************************************************************/
  /******************** STEP 1 : Initialize variables *************************/
  /****************************************************************************/
  const int maturityCount = 9;
  const int seriesCount = 12;

  // Define time to maturity
  std::array<double,maturityCount> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};

  // Define an array for Model Parameters e.g. alpha, beta, sigma
  std::array<double , seriesCount> alphaArray;
  std::array<double , seriesCount> betaArray;
  std::array<double , seriesCount> sigmaArray;
  std::array<double , seriesCount> errorArray;
  std::array<double , seriesCount> iterArray;
  std::array<double , seriesCount> timeArray;

  // Define the array that stores the final mdlData
  std::array< std::array< double, maturityCount>, seriesCount> mdlData;


  /****************************************************************************/
	/******************** STEP 2 : Read the Data ********************************/
	/****************************************************************************/
  // read the data from file e.g. for 12 month
  // it is a matrix named mrktData and has the size of
  // sereisCount * maturityCount with seriesCount as the number of month
  // and maturityCount as the number of maturities. e.g mrktData [12 * 9]
  // we also have an array tau [maturityCount] as the Time to maturity

  // TODO: think of reading the data without knowing the row size
    std::array<std::array<double,maturityCount>, seriesCount> mrktData;
    std::array<double,maturityCount> crrntMonthMrktData;

  // get the first argument of main as file name
  readData(argv[1], mrktData);

  /****************************************************************************/
	/*************************** STEP 2 : Run DE ********************************/
	/****************************************************************************/
  // Here we call the DE functions to run over the data
  // main DE function which is called runDE implments the DE algorithm and
  // calculates the Error for each time-serie we have

  // define the Differential Evolution object
  DE d(method);


  auto start = std::chrono::steady_clock::now();

  // Call the Differential Evolution Function
  // for each time-serie
  for(int i = 0; i < seriesCount; i++)
  {
    crrntMonthMrktData = mrktData[seriesCount-1-i];
    std::cout << "=============================" << std::endl;
    std::cout << "Running DE for Series number:" << i << std::endl;
    d.setMrktArray(crrntMonthMrktData);
    d.runDE();
    alphaArray[i] = d.getAlpha();
    betaArray[i] = d.getBeta();
    sigmaArray[i] = d.getSigma();
    errorArray[i] = d.getError();
    mdlData[i] = d.getMdlArray();
    iterArray[i] = d.getIter();
    timeArray[i] = d.getTime();
  }

  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> durationCount = end - start;
  double totalTime = durationCount.count();

  /****************************************************************************/
	/*************************** STEP 4 : Print Out *****************************/
	/****************************************************************************/

  for(int i = 0; i < seriesCount; i++)
  {
    std::cout << "\nfinal alpha:" <<  alphaArray[i] <<std::endl;
    std::cout << "final beta:" << betaArray[i] <<std::endl;
    std::cout << "final sigma:" << sigmaArray[i] <<std::endl;
    std::cout << "Average Error for month :" << i;
    std::cout << "\t is : " << errorArray[i] << std::endl;
    std::cout << "Elapsed Time: " << timeArray[i] << std::endl;
    std::cout << "Number of Iterations: " << iterArray[i] << std::endl;
    for (size_t j = 0; j < 9; j++) {
      std::cout << "y for maturity: "  << tau[j] << "\t is: \t" << mdlData[i][j] << std::endl;
    }
  }

  method = "Total" + method + "CPU" + std::to_string(seriesCount);
  writeData(mdlData, mrktData, alphaArray, betaArray, sigmaArray,
          errorArray, iterArray, timeArray,method);

          std::cout << "Data has been written to file" << std::endl;
          std::cout << "Total Calculation Time is: " << totalTime << std::endl;

  return 0;
}
