#include "../Headers/risklab.h"

namespace Calibration {




	void risklab::run()
	{

		const int maturityCount = 8; // TODO: think of this
		// std::array<double,9> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
		// double r0 = 0.0006;

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
		error = 0.0;
		for (int i = 0; i < maturityCount; i++)
		{
			error += std::pow((crrntMonthMdlData[i] - crrntMonthMrktData[i]),2) ;
		}
		error = error/maturityCount;

	} // risklab::run

	void risklab::getGDP()
	{

	 /******************** STEP 1 : Calculate r1 *********************************/
	 /****************************************************************************/
		const int scenarioCount = 1000; // should be eventually 10000
		std::array<double,scenarioCount> r1;

		for(int i = 0; i < scenarioCount; i++)
		{
			r1[i] = nextGDP();
		}

		// now we average the matrix for each maturity and
		double sum = 0.0;

			for(int i = 0; i < scenarioCount; i++)
			{
				sum += r1[i];
			}
			crrntMonthMdlGDP = sum/scenarioCount;
			sum = 0.0;

	// now that we have this average we can compare it
	// with crrntMonthMrktData which is e.g. Jan. 2015;
		gdpError = 0.0;

		gdpError += std::pow((crrntMonthMdlGDP - crrntMonthMrktGDP),2) ;


	} // risklab::getGDP

	double risklab::nextGDP()
	{
		double delta_r;
		double deltaT = 1.0/12.0;
		std::random_device rd;
	 	std::mt19937 gen(rd());
	 	std::normal_distribution<> d(0.0,1.0);
		std::normal_distribution<> s(0.0001,1.0);
		sigmaZ = s(gen);

		double randomVariable = d(gen);
		delta_r = alphaZ * (betaZ - r0) * deltaT + sigmaZ * std::sqrt(deltaT) * randomVariable;

		return r0 + delta_r;
	} // vasicek::nextGDP


	double risklab::nextRate()
	{
		double delta_r;
		double yT, zT;
		double deltaT = 1.0/12.0;
		std::random_device rd;
	 	std::mt19937 gen(rd());
	 	std::normal_distribution<> d(0.0,1.0);
		yT = alphaY * (betaY - y0) * deltaT + sigmaY * std::sqrt(deltaT) * d(gen);
		zT = alphaZ * (betaZ - z0) * deltaT + sigmaZ * std::sqrt(deltaT) * d(gen);
		delta_r = alphaS * (betaS - r0 + weightY * yT + weightZ * zT) * \
							deltaT + sigmaS * std::sqrt(deltaT) * d(gen);

		return r0 + delta_r;
	} // risklab::nextRate


	double risklab::getYield(double const& r1, double const& tau)
	{
		double yield;
		double A,B,bondPrice;

		// TODO: The formulations for A and B are taken from "Options, feauters
		// and other derivatives" book. "A" some times is so small that it becomes zero
		// that's why it might lead to a infinity yield value. that's why I made them
		// long double. maybe there is a better solution to that ....

		B = (1-std::exp(-alphaS*tau))/alphaS;
		A = std::exp(((B - tau)*(std::pow(alphaS,2)*betaS - 0.5*std::pow(sigmaS,2))\
		/std::pow(alphaS,2)) - (std::pow(sigmaS,2)*std::pow(B,2)/(4*alphaS)));

		bondPrice = A*std::exp(-r1*B);
		if (bondPrice == 0)	bondPrice = 0.000001;

		yield = ((-1/tau)*std::log(bondPrice));

		// TODO: A might be too small that makes yield to become infinity
		// I made it long double but still sometimes it becoms inf
		// if(yield == inf)	yield = 10;
		// if(yield == -inf) yield = -10;

		return yield;
	} //risklab::getYield


/****************************************************************************/
/******************** Setters and Getters are here **************************/
/****************************************************************************/
	risklab::risklab(double const& rZero,double const& yZero,
									 double const& zZero, std::array<double, 8> const& T)
	{
		r0 = rZero;
		y0 = yZero;
		z0 = zZero;
		tau = T;
	}
	risklab::risklab(double const& zZero)
	{
		z0 = zZero;
	}

	void risklab::setParameters(double const& as, double const& bs, double const& ss,
															double const& ay, double const& by, double const& sy,
															double const& sz, double const& wy, double const& wz)
	{
		alphaS = as;
		betaS = bs;
		sigmaS = ss;
		alphaY = ay;
		betaY = by;
		sigmaY = sy;
		sigmaZ = sz;
		weightY = wy;
		weightZ = wz;
	}

	void risklab::setGDPParameters(double const& az, double const& bz, double const& sz)
	{
		alphaZ = az;
		betaZ = bz;
		sigmaZ = sz;
	}

	void risklab::setMrktGDP(double const& mrktData)
	{
		crrntMonthMrktGDP = mrktData;
	}

	void risklab::setMrktArray(std::array<double, 8> const& mrktData)
	{
		crrntMonthMrktData = mrktData;
	}

	const double& risklab::getError() const
	{
		return error;
	}

	const double& risklab::getGDPError() const
	{
		return gdpError;
	}
	const double& risklab::getTheGDP() const
	{
		return crrntMonthMdlGDP;
	}

} // namespace Calibration
