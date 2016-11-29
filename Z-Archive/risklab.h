#pragma once

#include <vector>
#include <string>
#include <array>
#include <random>
#include <limits>
#include <algorithm>
#include <iostream>

namespace Calibration {


  class risklab{
    public:
      void run();
      double nextRate();
      double nextGDP();
      void getGDP();
      double getYield(double const& r1, double const& tau);

      risklab(double const& rZero,double const& yZero,
    									 double const& zZero, std::array<double, 8> const& T);
      risklab(double const& zZero);
      void setParameters(double const& as, double const& bs, double const& ss,
      									 double const& ay, double const& by, double const& sy,
      									 double const& sz, double const& wy, double const& wz);
      void setGDPParameters(double const& az, double const& bz, double const& sz);
      void setMrktArray(std::array<double, 8> const& mrktData);
      void setMrktGDP(double const& mrktData);
      const double& getError() const;
      const double& getGDPError() const;
      const double& getTheGDP() const;

    private:
      double alphaS, betaS, sigmaS, alphaY, betaY, sigmaY;
      double alphaZ, betaZ, sigmaZ;
      double weightY, weightZ;
      std::array<double, 8> crrntMonthMrktData;
      double crrntMonthMrktGDP, crrntMonthMdlGDP;
      std::array<double, 8> tau;
      double r0, y0, z0;
      double error, gdpError;
  };

}
