#include "../Headers/HullWhite.h"
// 
// int main()
// {
//   return 0;
// }

namespace Calibration {
double inf = std::numeric_limits<double>::infinity();


void HullWhite::run()
{

  const int maturityCount = 9; // TODO: think of this
 /****************************************************************************/
 /******************** STEP 1 : Calculate r1 & u1 ****************************/
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
  std::array<double,scenarioCount> u1;


  for(int i = 0; i < scenarioCount; i++)
  {
    generateRandoms();
    u1[i] = nextURate();
    r1[i] = nextRRate(u1[i]);
  }


  /****************************************************************************/
  /******************** STEP 2 : Get Model Yield ******************************/
  /****************************************************************************/

  std::array< std::array <double, maturityCount > , scenarioCount > y;
  for(int i = 0; i < scenarioCount; i++)
  {
    for (int j = 0; j < maturityCount; j++)
    {
      y[i][j] = getYield(r1[i], u1[i], tau[j]);
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

} // HullWhite::run


double HullWhite::generateRandoms()
{
    // Generate the random variables with correlation rho
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0.0,1.0);

    double normalVar1 = d(gen);
    double normalVar2 = d(gen);

    rRandom = normalVar1;
    uRandom = rho * normalVar1 + std::sqrt(1 - rho * rho) * normalVar2;
}


double HullWhite::nextRRate(double u1)
{
  //TODO: implement!
  // first we need to calculate theta function.
  // and for this we need F(0,T) which is actually which is R(0,T)
  // and R(0,T) is actually crrntMonthMrktData. I am just telling the story
  // of name confusions here!
  // for now we give F0T a general value
  //  consider T-t = 0.25 - 0.0 = 0.25
  double F0T = 0.0012;
  double deltaT = 1.0/12.0;
  double theta;
  theta = alpha1 * F0T + F0T/(0.25) + sigma1 * sigma1 / 2 / alpha1
                  * (1 - std::exp(-2 * alpha1 * 0.25));

  double r1;
  r1 = r0 + (theta + u1 - r0 * alpha1) * deltaT + sigma1
                              * std::sqrt(deltaT) * rRandom;

  return -1.0;
} // HullWhite::nextRRate

double HullWhite::nextURate()
{
  double u1;
  double deltaT = 1.0/12.0;
  u1 = sigma2 * std::sqrt(deltaT) * uRandom;

  return u1;
} // HullWhite::nextURate



double HullWhite::getYield(double const& r1, double const& u1, double const& tau)
{
  double bondPrice, yield;
  double B = getB(tau);
  double C = getC(tau);
  double A = getA(tau, B, C);

  bondPrice = A * std::exp(-B * r1 -C * u1);

  if (bondPrice == 0)	bondPrice = 0.000001;

  yield = ((-1/tau)*std::log(bondPrice));

  extern double inf;

  if(yield == inf)	yield = 10;
  if(yield == -inf) yield = -10;

  return yield;
}


/****************************************************************************/
/******************** Setters and Getters are here **************************/
/****************************************************************************/
  HullWhite::HullWhite(double const& rZero, std::array<double, 9> const& T)
	{
		r0 = rZero;
		tau = T;
	}

	void HullWhite::setParameters(double const& a1, double const& s1,
                     double const& a2, double const& s2,
                     double const& ro)
	{
		alpha1 = a1;
    sigma1 = s1;
    alpha2 = a2;
		sigma2 = s2;
    rho = ro;
	}

	void HullWhite::setMrktArray(std::array<double, 9> const& mrktData)
	{
		crrntMonthMrktData = mrktData;
	}

	const double& HullWhite::getError() const
	{
		return error;
	}

  double HullWhite::getA(double const& tau, double& B, double& C)
  {
    //TODO: how do I get P(0,T), P(0,t), and F(0,T)
    // for now use dummy values
    double P0T = 1.0;
    double P0t = 1.0;
    double F0T = 1.0;

    return -1.0;
    // in my case t should be zero so getting this part is challenging
    // I need to find the proper formula
  }

  double HullWhite::getB(double const& tau)
  {
    return 1.0/alpha1*(1-std::exp(-alpha1 * tau));
  }

  double HullWhite::getC(double const& tau)
  {
    double term1, term2, term3;

    term1 = 1.0/(alpha1-alpha2)/alpha1*std::exp(-alpha1*tau);
    term2 = -1.0/(alpha1-alpha2)/alpha2*std::exp(-alpha2*tau);
    term3 = 1.0/alpha1/alpha2;

    return term1 + term2 + term3;
  }


}//namespace Calibration
