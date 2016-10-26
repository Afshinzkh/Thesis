#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <random>
#include <limits>
#include <algorithm>


namespace Calibration {



  class  DE{
    public:
      void runDE(std::array<double, 9> const &crrntMonthMrktData);
  };

}
