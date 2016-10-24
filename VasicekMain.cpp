#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <random>


// Function declarations -- TODO: later to be added into header
double vasicekDescritize(double r0, double alpha, double beta, double sigma);
double vasicekYield(double r1, double alpha, double beta, double sigma, double tau);

int main()
{
	// Test The funcking code Here
	/* Test the Commenting */
	std::cout << "Hello Asshole" << std::endl;

	/************************************* STEP 1 : READ THE DATA ************************************/
	// read the data from file e.g. for 12 month
	// it is a matrix named mrktData and has the size of
	// sereisCount * maturityCount with seriesCount as the number of month
	// and maturityCount as the number of maturities. e.g mrktData [12 * 9]
	// we also have an array tau [maturityCount] as the Time to maturity

	const int maturityCount = 9; // TODO: think of this
	std::array<double,9> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
	// define the data coloumn 1 size bigger so it doesn't go ham while reading
	// TODO: think of reading the data without knowing the row size
	std::array<std::array<double,9>, 13> mrktData;

	std::ifstream dataFile("Data.csv");
	int row = 0;
	int col = 0;
	if(dataFile.is_open())
	{
		std::cout << "Data File is Opened" << std::endl;

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
	std::cout << "reached end of file" << std::endl;
	std::cout << "data[11][5] is :" << mrktData[11][4] << std::endl;


	// read the first row of mrktData wich would be for the first month e.g. Jan.2015
	// and put this into an array called crrntMonthMrktData with size of [1 * maturityCount]
	std::array<double,9> crrntMonthMrktData;
	crrntMonthMrktData = mrktData[11];
	std::cout << "data[5] is :" << crrntMonthMrktData[4] << std::endl;

	// we want to get an array to compare it with crrntMonthMrktData
	// Initialize r0 to a given value;
	double r0 = 0.0006;

	// Initialize 3 random values for mean reversion rate :	    alpha
									  // long term mean :		beta
									  // volatility : 			sigma
 // TODO: Take the values from DE algorithm
	double alpha = 3.0;
	double beta = 0.0035;
	double sigma = 0.5;

	// Use Monte Carlo idea for vasicek/risklab Descritization to calculate r1
	// Here we call the vasicekDescritize Function with alpha, beta, sigma, r0 as inputs
	// and output would be a vector of r1 with scenarioCount: number of scenarios
	// r1 [scenarioCount * 1]
	// if we consider r0 as the short rate for previous month e.g. Dec. 2014
	// then r1 would be actually the short interest rate for the next month
	// which is our current Month e.g. Jan. 2015
	const int scenarioCount = 20; // should be eventually 10000
	std::array<double,scenarioCount> r1;
	// here we call the vasicekDescritize or risklabDescritize Function to get the short rates r1
	// TODO: take out of loop and vectorize instead
	for(int i = 0; i < scenarioCount; i++)
	{
		r1[i] = vasicekDescritize(r0,alpha,beta,sigma);
	}


	// Now that we have r1 we can use it to calculate the yield for each maturity;
	// remember maturityCount is the number of maturities, so we will have a matrix of
	// y [scenarioCount * maturityCount] as it is calulating yield for e.g. 9 different maturities
	// and e.g. 10000 different scenarios
	// we use vasicekYield/risklabYield formula to get these values
	// inputs are tau, alpha, beta, sigma, r1 ; output is yield
	std::array< std::array <double, maturityCount > , scenarioCount > y;
	for(int i = 0; i < scenarioCount; i++)
	{
		for (int j = 0; j < maturityCount; j++)
		{
			y[i][j] = vasicekYield(r1[i], alpha, beta, sigma, tau[j]);
		}
	}
	// now we average the matrix for each maturity and we get our final model yield for the current month;
	// auto crrntMonthMdlData = new double[1 * maturityCount];
	double sum = 0.0;
	std::array<double, maturityCount> crrntMonthMdlData;

	for (int j = 0; j < maturityCount; j++)
	{
		for(int i = 0; i < scenarioCount; i++)
		{
			sum += y[i][j]; // TODO: look for eigen matrix for 2D
		}
		crrntMonthMdlData[j] = sum/scenarioCount;
		sum = 0.0;
	}

	for (int j = 0; j < maturityCount; j++)
	{
		std::cout << "Model: " <<  crrntMonthMdlData[j] ;
		std::cout << "\t Market:" <<  crrntMonthMrktData[j] << std::endl;
	}


	// now that we have this average we can compare it with crrntMonthMrktData which is e.g. Jan. 2015;
	double error = 0.0;
	for (int i = 0; i < maturityCount; i++)
	{
		// error += (crrntMonthMdlData[i] - crrntMonthMrktData[i]) ^ 2 ;
	}

	// check the error
	// if (error < tolerance)
		// take values of alpha, beta, sigma
		// pit them into their assigned arrayes
		// a[1] = ... b[1] = ... s[1] = ...
		// and copy the crrntMonthMdlData to our finalYields which is a matrix of e.g. [12 * 9]
		// and here we just fill in for the first row e.g. jan 2015;
		// and then go for the next month e.g. Feb.15
	// else
		// try another alpha, beta, sigma, r0
		return 0;
}

double vasicekDescritize(double r0, double alpha, double beta, double sigma)
{
		double delta_r;
		double deltaT = 1.0/12.0;
		std::random_device rd;
	 	std::mt19937 gen(rd());
	 	std::normal_distribution<> d(0,1);
		double randomVariable = d(gen);
		// make delta_r with one step deltaT = 1/12;
		delta_r = alpha * (beta - r0) * deltaT + sigma * std::sqrt(deltaT) * randomVariable;

		return r0 + delta_r;
}

double vasicekYield(double r1, double alpha, double beta, double sigma, double tau)
{
		double yield;
		double A,Apart,B,bondPrice;

		B = (1-std::exp(-alpha*tau))/alpha;
		Apart = std::exp((B - tau)*(beta - (std::pow(sigma,2)/(2*std::pow(alpha, 2)))));
		A = Apart - std::pow(sigma,2)*std::pow(alpha,2)/(4*alpha);
		bondPrice = A*std::exp(-r1*B);

		yield = (std::pow(-tau,-1))*std::log(bondPrice);

		return yield;
}
