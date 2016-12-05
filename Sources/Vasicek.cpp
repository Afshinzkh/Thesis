#include "../Headers/Vasicek.h"

namespace Calibration {


double inf = std::numeric_limits<double>::infinity();



//TODO: rNext should be calculated for each month so what should i do is:
// when I run DE for each month, there I have to have an rNext


	void Vasicek::run()
	{

		const int maturityCount = 9;
		// std::array<double,9> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
		// double r0 = 0.0006;

		// Initialize 3 random values for mean reversion rate :	alpha
		// long term mean :		beta
		// volatility : 			sigma
	 // Values are Taken from DE algorithm

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


	double Vasicek::nextRate()
	{

		double deltaT = 1.0/12.0;
		const int scenarioCount = 10000; // should be eventually 10000
		std::array<double,scenarioCount> delta_r;
		std::array<double,scenarioCount> randomArray;
		// here we call the vasicekDescritize or risklabDescritize Function
		//  to get the short rates rate

		std::random_device rd;
	 	std::mt19937 gen(rd());
	 	std::normal_distribution<> d(0.0,1.0);
		for (int i = 0; i < scenarioCount; ++i) {
				randomArray[i] = d(gen);
		}

		// make delta_r with one step deltaT = 1/12;
		int index = 0;
		double rSum = 0.0;
		for(auto& num: delta_r)
		{
		 delta_r[index] = alpha * (beta - rNext) * deltaT + sigma * std::sqrt(deltaT) * randomArray[index];
		 ++index;
		 rSum += delta_r[index];
	 }
		//  rate = rNext + delta_r;
		double deltaR = rSum / scenarioCount;

		rNext = rNext + deltaR;
		return rNext;
	} // vasicek::nextRate


	double Vasicek::getYield(double const& tau)
	{
		double yield;
		double A,B,bondPrice;

		// TODO: The formulations for A and B are taken from "Options, feauters
		// and other derivatives" book. "A" some times is so small that it becomes zero
		// that's why it might lead to a infinity yield value. that's why I made them
		// long double. maybe there is a better solution to that ....

		B = (1-std::exp(-alpha*tau))/alpha;
		A = std::exp(((B - tau)*(std::pow(alpha,2)*beta - 0.5*std::pow(sigma,2))\
		/std::pow(alpha,2)) - (std::pow(sigma,2)*std::pow(B,2)/(4*alpha)));

		bondPrice = A*std::exp(-rNext*B);
		if (bondPrice == 0)	bondPrice = 0.000001;

		yield = ((-1/tau)*std::log(bondPrice));

		// TODO: A might be too small that makes yield to become infinity
		// I made it long double but still sometimes it becoms inf
		if(yield == inf)	yield = 10;
		if(yield == -inf) yield = -10;

		return yield;
	} //Vasicek::getYield


/****************************************************************************/
/******************** Setters and Getters are here **************************/
/****************************************************************************/
	Vasicek::Vasicek(double const& rZero, std::array<double, 9> const& T)
	{
		rNext = rZero;
		tau = T;
	}

	void Vasicek::setParameters(double const& a, double const& b, double const& s)
	{
		alpha = a;
		beta = b;
		sigma = s;
	}

	void Vasicek::setMrktArray(std::array<double, 9> const& mrktData)
	{
		crrntMonthMrktData = mrktData;
	}

	const double& Vasicek::getError() const
	{
		return error;
	}


} // namespace Calibration
