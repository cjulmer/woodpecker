#pragma once

// Moose
#include "MooseRandom.h"

#include <fstream>

class RandomTestSub
{
public:
  //RandomTestSub() {};
  RandomTestSub(MooseRandom &);
protected:
  MooseRandom & _random;
  std::fstream _debug;
};

RandomTestSub::RandomTestSub(MooseRandom & input)
  : _random(input)
{
  _debug.open("Random2.txt", std::ofstream::out | std::ofstream::app);
  for (auto i = 0; i < 3; ++i)
  {
    _debug << _random.rand(0) << "\n";
  }
  _debug.close();
}
