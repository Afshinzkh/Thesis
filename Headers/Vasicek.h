#pragma once

#include <vector>
#include <string>
#include <array>
#include <random>
#include <limits>
#include <algorithm>


namespace Calibration {


  class Vasicek{
    public:
      double run(double alpha, double beta, double sigma,
              std::array<double, 9> const &crrntMonthMrktData);
      double descritize(double r0, double alpha, double beta, double sigma);
      double getYield(double r1, double alpha, double beta, double sigma, double tau);
  };

}
