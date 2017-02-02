#include "../Headers/Vasicek.h"

namespace Calibration {


double inf = std::numeric_limits<double>::infinity();

void Vasicek::run()
{

		const int maturityCount = 9;

	 /****************************************************************************/
	 /******************** STEP 1 : Get next Rate ********************************/
	 /****************************************************************************/
	 	// This step has been moved to de.cpp where we call nextRate function
		/****************************************************************************/
		/******************** STEP 2 : Get Model Yield ******************************/
		/****************************************************************************/
		// Now that we have r1 we can use it to calculate the yield for each maturity;
		// remember maturityCount is the number of maturities, so we will have a matrix of
		std::array<double, maturityCount> crrntMonthMdlData;

		for (int i = 0; i < maturityCount; ++i)
		{
				crrntMonthMdlData[i] = getYield(tau[i]);
		}


	/****************************************************************************/
	/******************** STEP 3 : Get THE Error ********************************/
	/****************************************************************************/
	// now that we have this average we can compare it
	// with crrntMonthMrktData which is e.g. Jan. 2015;
		error = 0.0;
		for (int i = 0; i < maturityCount; i++)
		{
			error += (crrntMonthMdlData[i] - crrntMonthMrktData[i])
									 	* (crrntMonthMdlData[i] - crrntMonthMrktData[i]);
		}
		error = error/maturityCount;

	} // Vasicek::run


	void Vasicek::nextRate()
	{
		// Monte carlo method is used here
		// we generate e.g. 10000 random variables
		// and then we calculate 10000 delta_r using vasicek short rate model
		// we average the delta_r and add it to rZero
		// this would be our next short rate or rNext

		double deltaT = 1.0/12.0;
		const int scenarioCount = 10000;
		std::array<double,scenarioCount> delta_r;
		std::array<double,scenarioCount> randomArray;

		// Generate random Array with N(0,1)
		std::random_device rd;
	 	std::mt19937 gen(rd());
	 	std::normal_distribution<> d(0.0,1.0);
		for (int i = 0; i < scenarioCount; ++i) {
				randomArray[i] = d(gen);
		}

		int index = 0;
		double rSum = 0.0;
		for(auto& num: delta_r)
		{
		 delta_r[index] = alpha * (beta - r0) * deltaT + sigma * std::sqrt(deltaT) * randomArray[index];
		 ++index;
		 rSum += delta_r[index];
	 }
		double deltaR = rSum / scenarioCount;

		rNext = r0 + deltaR;
	} // vasicek::nextRate


	double Vasicek::getYield(double const& tau)
	{
		double yield;
		double A,B,bondPrice;

		B = (1.0-std::exp(-alpha*tau))/alpha;
		A = std::exp(((B - tau)*(std::pow(alpha,2)*beta - 0.5*std::pow(sigma,2))\
		/std::pow(alpha,2)) - (std::pow(sigma,2)*std::pow(B,2)/(4*alpha)));

		bondPrice = A*std::exp(-rNext*B);
		if (bondPrice == 0)	bondPrice = 0.000001;

		yield = ((-1.0/tau)*std::log(bondPrice));

		if(yield == inf)	yield = 10;
		if(yield == -inf) yield = -10;

		return yield;
	} //Vasicek::getYield


} // namespace Calibration
