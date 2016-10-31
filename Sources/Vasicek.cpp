#include "../Headers/Vasicek.h"

namespace Calibration {


double inf = std::numeric_limits<double>::infinity();


	void Vasicek::run()
	{

		const int maturityCount = 9; // TODO: think of this
		// std::array<double,9> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
		// double r0 = 0.0006;

		// Initialize 3 random values for mean reversion rate :	alpha
		// long term mean :		beta
		// volatility : 			sigma
	 // Values are Taken from DE algorithm
	 /****************************************************************************/
	 /******************** STEP 1 : Calculate r1 *********************************/
	 /****************************************************************************/
		// Use Monte Carlo idea for vasicek/risklab Descritization to calculate r1
		// Here we call the vasicekDescritize Function with alpha, beta, sigma, r0 as inputs
		// and output would be a vector of r1 with scenarioCount: number of scenarios
		// r1 [scenarioCount * 1]
		// if we consider r0 as the short rate for previous month e.g. Dec. 2014
		// then r1 would be actually the short interest rate for the next month
		// which is our current Month e.g. Jan. 2015
		const int scenarioCount = 1000; // should be eventually 10000
		std::array<double,scenarioCount> r1;
		// here we call the vasicekDescritize or risklabDescritize Function
		//  to get the short rates r1
		// TODO: take out of loop and vectorize instead
		// std::array < double, scenarioCount> alphaArray;
		// std::array < double, scenarioCount> betaArray;
		// std::array < double, scenarioCount> sigmaArray;
		// std::array < double, scenarioCount> r0Array;
		// alphaArray.fill(alpha);
		// betaArray.fill(beta);
		// sigmaArray.fill(sigma);
		// r0Array.fill(r0);

		for(int i = 0; i < scenarioCount; i++)
		{
			r1[i] = nextRate();
		}


		/****************************************************************************/
		/******************** STEP 2 : Get Model Yield ******************************/
		/****************************************************************************/
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
				y[i][j] = getYield(r1[i], tau[j]);
			}
		}
		// now we average the matrix for each maturity and
		// we get our final model yield for the current month;
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


	/****************************************************************************/
	/******************** STEP 3 : Get THE Error ********************************/
	/****************************************************************************/
	// now that we have this average we can compare it
	// with crrntMonthMrktData which is e.g. Jan. 2015;
		double error = 0.0;
		for (int i = 0; i < maturityCount; i++)
		{
			error += std::pow((crrntMonthMdlData[i] - crrntMonthMrktData[i]),2) ;
		}
		error = error/maturityCount;

	} // Vasicek::run


	double Vasicek::nextRate()
	{
		double delta_r;
		double deltaT = 1.0/12.0;
		std::random_device rd;
	 	std::mt19937 gen(rd());
	 	std::normal_distribution<> d(0.0,1.0);

		double randomVariable = d(gen);
		// long double r1 = r0 * std::exp(-alpha*deltaT) + beta * (1 - std::exp(-alpha*deltaT))\
		// + sigma * std::sqrt((1 - std::exp(-2*alpha*deltaT))/(2*alpha)) * randomVariable;
		// make delta_r with one step deltaT = 1/12;
		delta_r = alpha * (beta - r0) * deltaT + sigma * std::sqrt(deltaT) * randomVariable;

		return r0 + delta_r;
	} // vasicek::nextRate


	double Vasicek::getYield(double const& r1, double const& tau)
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

		bondPrice = A*std::exp(-r1*B);
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
		r0 = rZero;
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
