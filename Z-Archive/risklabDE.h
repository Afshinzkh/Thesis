#pragma once

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <chrono>


namespace Calibration {

  class  risklabDE{
    public:
      void runDE();
      void runGDPDE();
      risklabDE();
      const double& getAlphaS() const;
      const double& getBetaS() const;
      const double& getSigmaS() const;
      const double& getAlphaY() const;
      const double& getBetaY() const;
      const double& getSigmaY() const;
      const double& getAlphaZ() const;
      const double& getBetaZ() const;
      const double& getSigmaZ() const;
      const double& getWeightY() const;
      const double& getWeightZ() const;

      const double& getError() const;
      const double& getGDPError() const;
      const int& getIter() const;
      const double& getTime() const;
      void setMrktArray(std::array<double, 8> const& mrktData);
      void setMrktGDP(double const& mrktData);

      const std::array<double, 8>& getMdlArray() const;

    private:
      double alphaS, betaS, sigmaS, alphaY, betaY, sigmaY;
      double alphaZ, betaZ, sigmaZ;
      double weightY, weightZ;
      double avgError, gdpError;
      int loopCount;
      double calTime;
      std::array<double, 8> crrntMonthMrktData;
      std::array<double, 8> crrntMonthMdlData;
      double MrktGDP;
      std::string methodName;
  };

}
