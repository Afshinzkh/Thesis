#include<iostream>
using namespace std;

int main()
{
	// Test The funcking code Here
	/* Test the Commenting */
	cout << "Hello Asshole" << endl;


	/************************************* STEP 1 : READ THE DATA ************************************/
	// read the data from file e.g. for 12 month
	// it is a matrix named mrktData and has the size of 
	// sereisCount * maturityCount with seriesCount as the number of month
	// and maturityCount as the number of maturities. e.g mrktData [12 * 9]  
	// we also have an array tau [maturityCount] as the Time to maturity
	
	//auto tau = new double [0.25, 1, 3, 5, 7, 10, 15, 20, 25, 30]; // think of defining arrays with std::array
	int maturityCount = 9; // think of this 

	// read the first row of mrktData wich would be for the first month e.g. Jan.2015
	// and put this into an array called crrntMonthMrktData with size of [1 * maturityCount]
	

	// we want to get an array to compare it with crrntMonthMrktData
	// Initialize r0 to a given value;
	double r0 = 0.0001;

	// Initialize 3 random values for mean reversion rate :	    alpha
									  // long term mean :		beta
									  // volatility : 			sigma
	double alpha = 1.0;
	double beta = 2.0;
	double sigma = 3.0;

	// Use Monte Carlo idea for vasicek/risklab Descritization to calculate r1
	// Here we call the vasicekDescritize Function with alpha, beta, sigma, r0 as inputs
	// and output would be a vector of r1 with scenarioCount: number of scenarios
	// r1 [scenarioCount * 1]
	// if we consider r0 as the short rate for previous month e.g. Dec. 2014
	// then r1 would be actually the short interest rate for the next month 
	// which is our current Month e.g. Jan. 2015
	int scenarioCount = 10000;
	//double r1 = new double[scenarioCount * 1];

	// here we call the vasicekDescritize or risklabDescritize Function to get the short rates r1
	for(int i = 0; i < scenarioCount; i++)
	{
		//r1[i] = vasicekDescritize(r0,alpha,beta,sigma);
	}


	// Now that we have r1 we can use it to calculate the yield for each maturity;
	// remember maturityCount is the number of maturities, so we will have a matrix of
	// y [scenarioCount * maturityCount] as it is calulating yield for e.g. 9 different maturities 
	// and e.g. 10000 different scenarios
	// we use vasicekYield/risklabYield formula to get these values
	// inputs are tau, alpha, beta, sigma, r1 ; output is yield 
	
	for(int i = 0; i < scenarioCount; i++)
	{
		for (int j = 0; j < maturityCount; j++)
		{
			//y[i][j] = vasicekYield()
		}
	}

	// now we average the matrix for each maturity and we get our final model yield for the current month;
	// auto crrntMonthMdlData = new double[1 * maturityCount];
	double sum = 0.0;
	for (int j = 0; j < maturityCount; j++)
	{
		for(int i = 0; i < scenarioCount; i++)
		{
			//sum += y[i][j]; // look for eigen matrix for 2D 
		}
		// nextMonthYield[j] = sum/scenarioCount;
		// sum = 0.0;
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
		
}