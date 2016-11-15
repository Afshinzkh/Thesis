#include "../Headers/risklab.h"
#include "../Headers/risklabDE.h"
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

  std::cout << "Method to use: Risklab" << std::endl;



  /****************************************************************************/
  /******************** STEP 1 : Initialize variables *************************/
  /****************************** for GDP *************************************/
  // TODO: for now time-series length is 12 since we wanna get the values
  // for 12 months and maturity is 9 according to our data
  // Maturity [0.25, 1, 3, 5, 7, 10, 15, 20, 30]
  const int maturityCount = 9;
  // // Jan 2015 bis Dec. 2015
  const int seriesCount = 12;

  // Initialize variables for GDP TODO: read from file later
  std::vector < double > mrktGDP(12);
  mrktGDP = {0.015, 0.018, 0.01, 0.025, 0.02, 0.009,
             0.023, 0.028, 0.019, 0.018, 0.011, 0.012};

  // Define an array for Model Parameters e.g. alphaZ, betaZ
  std::vector<double> alphaZ(seriesCount);
  std::vector<double> betaZ(seriesCount);
  /****************************** for Spread *************************************/
  // Define time to maturity
  std::vector<double> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
  // std::cout << "Tau is " << tau[2] << std::endl;

  // Define an array for Model Parameters
  // that are 9 : alphaS, betaS, sigmaS, alphaY, betaY, sigmaY,
  // sigmaZ, bY, bZ
  std::vector<double> alphaS(seriesCount);
  std::vector<double> betaS(seriesCount);
  std::vector<double> sigmaS(seriesCount);
  std::vector<double> alphaY(seriesCount);
  std::vector<double> betaY(seriesCount);
  std::vector<double> sigmaY(seriesCount);
  std::vector<double> sigmaZ(seriesCount);
  std::vector<double> weightY(seriesCount);
  std::vector<double> weightZ(seriesCount);

  std::vector<double> errorVec(seriesCount);
  std::vector<double> iterVec(seriesCount);
  std::vector<double> timeVec(seriesCount);

  // Define the array that stores the final mdlData
  std::vector<std::vector<double>> mdlData(seriesCount, std::vector<double> (maturityCount,0));

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

  // TODO: here you have to get the first argument of main as file name
  readData(argv[1], mrktData);
  /****************************************************************************/
	/*************************** STEP 2 : Run DE ********************************/
	/****************************************************************************/

  risklabDE d;

  // for each time-serie
  for(int i = 0; i < seriesCount; i++)
  {

    // Fisrt Get the parameters for GDP
    std::cout << "=============================" << std::endl;
    d.setMrktGDP(mrktGDP[i]);
    d.runGDPDE();
    alphaZ[i] = d.getAlphaZ();
    betaZ[i] = d.getBetaZ();

    // now go for the big DE
    crrntMonthMrktData = mrktData[seriesCount-1-i];
    std::cout << "crrntMonthMrktData[2] = " << crrntMonthMrktData[2] << std::endl;
    d.setMrktArray(crrntMonthMrktData);
    d.runDE();
    std::cout << "*************** RAN ONE TIME **************" << '\n';
    alphaS[i] = d.getAlphaS();
    betaS[i] = d.getBetaS();
    sigmaS[i] = d.getSigmaS();
    alphaY[i] = d.getAlphaY();
    betaY[i] = d.getBetaY();
    sigmaY[i] = d.getSigmaY();
    sigmaZ[i] = d.getSigmaZ();
    weightY[i] = d.getWeightY();
    weightZ[i] = d.getWeightZ();



  }



  /****************************************************************************/
	/*************************** STEP 4 : Print Out *****************************/
	/****************************************************************************/

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
  // writeData(mdlData, mrktData, alphaArray, betaArray, sigmaArray,
  //         errorArray, iterArray, timeArray);


  return 0;
}
