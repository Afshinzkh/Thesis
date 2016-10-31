#pragma once

#include <vector>
#include <string>
#include <array>
#include <random>
#include <limits>
#include <algorithm>
#include <iostream>

namespace Calibration {


  class Vasicek{
    public:
      double run();
      double nextRate();
      double getYield(double const& r1, double const& tau);

      Vasicek(double const& rZero, std::array<double, 9> const& T );
      void setParameters(double const& a, double const& b, double const& s);
      void setMrktArray(std::array<double, 9> const& mrktData);
      double getError();

    private:
      double alpha, beta, sigma;
      std::array<double, 9> crrntMonthMrktData;
      std::array<double, 9> tau;
      double r0;
      double error;
  };

}
