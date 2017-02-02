#pragma once

#include "../Headers/Method.h"
#include <vector>
#include <string>
#include <array>
#include <random>
#include <limits>
#include <algorithm>
#include <iostream>

namespace Calibration {


  class CIR : public Method{
    public:
      virtual void run();
      virtual void nextRate();
      virtual double getYield(double const& tau);

      CIR(double const& rZero, std::array<double, 9> const& T ) : Method(rZero, T) {}
  };

}
