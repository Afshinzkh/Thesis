#pragma once

#include <vector>
#include <string>
#include <array>
#include <random>
#include <limits>
#include <algorithm>
#include <iostream>

namespace Calibration {


  class Method{
    public:
      virtual void run() = 0;
      virtual void nextRate() = 0;
      virtual double getYield(double const& tau) = 0;

      Method(double const& rZero, std::array<double, 9> const& T ){
        r0 = rZero;
    		tau = T;
      }
      void setParameters(double const& a, double const& b, double const& s)
      {
        alpha = a;
    		beta = b;
    		sigma = s;
      }
      void setMrktArray(std::array<double, 9> const& mrktData)
      {
        crrntMonthMrktData = mrktData;
      }
      const double& getError() const{return error;}
      const double& getNewR() const {return rNext;}

    protected:
      double alpha, beta, sigma;
      std::array<double, 9> crrntMonthMrktData;
      std::array<double, 9> tau;
      double r0, rNext;
      double error;
  };

}
