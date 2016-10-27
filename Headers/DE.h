#pragma once

#include <iostream>
#include <string>
#include <array>
#include <random>


namespace Calibration {

  class  DE{
    public:
      template < size_t N >
      void runDE(std::string methodName,
            std::array<double, N> const &crrntMonthMrktData);
  };

}

#include "../Files/DE.tpp"
