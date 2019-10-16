#pragma once

// Moose
#include "GeneralUserObject.h"

// Forward declaration
class RandomTest;
class MooseRandom;

template <>
InputParameters validParams<RandomTest>();

class RandomTest : public GeneralUserObject
{
public:
  RandomTest(const InputParameters & parameters);

  virtual void initialize() override{};
  virtual void finalize() override{};
  virtual void execute() override;

protected:
  MooseRandom & _random;
  std::fstream _debug;
};
