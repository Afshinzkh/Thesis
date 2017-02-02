#pragma once

#include <vector>
#include <string>
#include <array>
#include <random>
#include <limits>
#include <algorithm>
#include <iostream>

namespace Calibration {


  class HullWhite{
    public:
      void run();
      double generateRandoms();
      double nextRRate(double u1);
      double nextURate();

      double getYield(double const& r1, double const& u1, double const& tau);
           HullWhite(double const& rZero, std::array<double, 9> const& T);
      void setParameters(double const& a1, double const& s1,
                         double const& a2, double const& s2,
                         double const& ro);
      void setMrktArray(std::array<double, 9> const& mrktData);
      const double& getError() const;

      double getA(double const& tau, double& B, double& C);
      double getB(double const& tau);
      double getC(double const& tau);

    private:
      double alpha1, sigma1, alpha2, sigma2, rho;
      double rRandom, uRandom;
      std::array<double, 9> crrntMonthMrktData;
      std::array<double, 9> tau;
      double r0;
      double error;
  };

}
