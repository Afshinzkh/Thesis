#include "../Headers/HullWhite.h"
#include "../Headers/hwDE.h"
#include "../Headers/Helper.h"

using namespace Calibration;

// In the main we go through each time-serie and get the optimized model
// parameters as well as the final yield curve for each time-serie
int main(int argc, char* argv[])
{

  // Cheking the Arguments
  if( argc != 2){
    std::cout << "Error: Wrong number of Arguments" << std::endl;
    return -1;
  }

  std::cout << "Method to use: HullWhite" << std::endl;



  /****************************************************************************/
  /******************** STEP 1 : Initialize variables *************************/
  // TODO: for now time-series length is 12 since we wanna get the values
  // for 12 months and maturity is 9 according to our data
  // Maturity [0.25, 1, 3, 5, 7, 10, 15, 20, 30]
  const int maturityCount = 9;
  // // Jan 2015 bis Dec. 2015
  const int seriesCount = 12;

  // Define time to maturity
  std::vector<double> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
  // std::cout << "Tau is " << tau[2] << std::endl;

  // Define an array for Model Parameters
  // that are 9 : alphaS, betaS, sigmaS, alphaY, betaY, sigmaY,
  // sigmaZ, bY, bZ
  std::array<double , seriesCount> alpha1;
  std::array<double , seriesCount> sigma1;
  std::array<double , seriesCount> alpha2;
  std::array<double , seriesCount> sigma2;
  std::array<double , seriesCount> rho;

  std::array<double , seriesCount> errorArray;
  std::array<double , seriesCount> iterArray;
  std::array<double , seriesCount> timeArray;

  // Define the array that stores the final mdlData
  // std::vector<std::vector<double>> mdlData(seriesCount, std::vector<double> (maturityCount,0));
  std::array<std::array<double,maturityCount>, seriesCount> mdlData;
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
  // An array of [1 * maturityCount] that holds only the current month values
    std::array<double,maturityCount> crrntMonthMrktData;

  readData(argv[1], mrktData);
  /****************************************************************************/
	/*************************** STEP 2 : Run DE ********************************/
	/****************************************************************************/
  double r0 = 0.0006;
  // this varibale the the next shortRate which is taken from last monthNames
  // at first it is r0 and then it gets updated for each month
  double newR = r0;


  hwDE d;

  // for each time-serie
  for(int i = 0; i < seriesCount; i++)
  {
    auto start = std::chrono::steady_clock::now();

    // now go for the big DE
    crrntMonthMrktData = mrktData[seriesCount-1-i];
    d.setMrktArray(crrntMonthMrktData);
    newR = d.runDE(newR);
    mdlData[i] = d.getMdlArray();
    alpha1[i] = d.getAlpha1();
    sigma1[i] = d.getSigma1();
    alpha2[i] = d.getAlpha2();
    sigma2[i] = d.getSigma2();
    rho[i] = d.getRho();
    errorArray[i] = d.getError();
    iterArray[i] = d.getIter();

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> durationCount = end - start;
    timeArray[i] = durationCount.count();

  }



  /****************************************************************************/
	/*************************** STEP 4 : Print Out *****************************/
	/****************************************************************************/
  //TODO: print out the results

  // for(int i = 0; i < seriesCount; i++)
  // {
  //   std::cout << "\nfinal alpha:" <<  alphaArray[i] <<std::endl;
  //   std::cout << "final beta:" << betaArray[i] <<std::endl;
  //   std::cout << "final sigma:" << sigmaArray[i] <<std::endl;
  //   std::cout << "Average Error for month :" << monthNames[i];
  //   std::cout << "\t is : " << errorArray[i] << std::endl;
  //
  //   for (size_t j = 0; j < 9; j++) {
  //     std::cout << "y for maturity: "  << tau[j] << "\t is: \t" << mdlData[i][j] << std::endl;
  //   }
  // }
  //
  writeHullWhiteData(mdlData, mrktData, alpha1, sigma1, alpha2, sigma2, rho,
                                      errorArray, iterArray, timeArray);


  return 0;
}
