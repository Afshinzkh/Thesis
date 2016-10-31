#pragma once

#include <iostream>
#include <string>
#include <array>
#include <random>


namespace Calibration {

  class  DE{
    public:
      void runDE();
      DE(std::string const& mName);
      const double& getAlpha() const;
      const double& getBeta() const;
      const double& getSigma() const;
      void setMrktArray(std::array<double, 9> const& mrktData);
      const std::array<double, 9>& getMdlArray() const;

    private:
      double alpha, beta, sigma;
      std::array<double, 9> crrntMonthMrktData;
      std::array<double, 9> crrntMonthMdlData;
      std::string methodName;
  };

}
