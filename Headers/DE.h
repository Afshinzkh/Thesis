#pragma once

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <chrono>


namespace Calibration {

  class  DE{
    public:
      void runDE();
      DE(std::string const& mName);
      const double& getAlpha() const;
      const double& getBeta() const;
      const double& getSigma() const;
      const double& getError() const;
      const int& getIter() const;
      const double& getTime() const;
      void setMrktArray(std::array<double, 9> const& mrktData);
      const std::array<double, 9>& getMdlArray() const;

    private:
      double alpha, beta, sigma, avgError;
      int loopCount;
      double calTime;
      std::array<double, 9> crrntMonthMrktData;
      std::array<double, 9> crrntMonthMdlData;
      std::string methodName;
  };

}
