#pragma once

#include <iostream>
#include <string>
#include <array>
#include <random>


namespace Calibration {

  class  DE{
    public:
      void runDE(std::string methodName,
            std::array<double, 9> const &crrntMonthMrktData,
            std::array<double, 9> &crrntMonthMdlData,
          double *alpha, double *beta, double *sigma);
  };

}
