#pragma once

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <chrono>


namespace Calibration {

  class  hwDE{
    public:
      double runDE( double newR);
      hwDE();
      const double& getAlpha1() const;
      const double& getSigma1() const;
      const double& getAlpha2() const;
      const double& getSigma2() const;
      const double& getRho() const;

      const double& getError() const;
      const int& getIter() const;
      const double& getTime() const;
      void setMrktArray(std::array<double, 9> const& mrktData);
      void setMrktGDP(double const& mrktData);

      const std::array<double, 9>& getMdlArray() const;

    private:
      double alpha1, sigma1, alpha2, sigma2, rho;
      double avgError, gdpError;
      int loopCount;
      double calTime;
      std::array<double, 9> crrntMonthMrktData;
      std::array<double, 9> crrntMonthMdlData;
      std::string methodName;
  };

}
