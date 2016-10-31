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

  if(argv[2] != std::string("vasicek") && argv[2] != std::string("risklab")  )
  {
    std::cout << "Error: Wrong Method Name" << std::endl;
    return -1;
  }

  std::cout << "Method to use: " << argv[2] << std::endl;



  /****************************************************************************/
  /******************** STEP 1 : Initialize variables *************************/
  /****************************************************************************/
  // TODO: for now time-series length is 12 since we wanna get the values
  // for 12 months and maturity is 9 according to our data
  // Maturity [0.25, 1, 3, 5, 7, 10, 15, 20, 30]
  const int maturityCount = 9;
  // Jan 2015 bis Dec. 2015
  const int seriesCount = 12;

  // Define time to maturity
  std::array<double,maturityCount> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};

  // Define an array for Model Parameters e.g. alpha, beta, sigma
  std::array<double , seriesCount> alphaArray;
  std::array<double , seriesCount> betaArray;
  std::array<double , seriesCount> sigmaArray;

  // Define the array that stores the final modelYields
  std::array< std::array< double, maturityCount>, seriesCount> modelYields;

  /****************************************************************************/
	/******************** STEP 1 : Read the Data ********************************/
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
  // Here we call the DE functions to run over the data
  // main DE function which is called runDE implments the DE algorithm and
  // calculates the Error for each time-serie we have

  // define the Differential Evolution object
  DE d;

  // This is just for current beauty
  std::array <std::string, seriesCount> monthNames = {
    "Jan.2015", "Feb.2015", "Mar.2015", "Apr.2015", "May.2015",
    "Jun.2015", "Jul.2015", "Aug.2015", "Sep.2015", "Oct.2015",
    "Nov.2015", "Dec.2015"
  };

  // Call the Differential Evolution Function
  // for each time-serie
  for(int i = 0; i < seriesCount; i++)
  {
    crrntMonthMrktData = mrktData[seriesCount-1-i];
    std::cout << "=============================" << std::endl;
    std::cout << "Running DE for :" << monthNames[i] << std::endl;
    d.runDE(argv[2], crrntMonthMrktData); // in DE.tpp file
  }


  return 0;
}
