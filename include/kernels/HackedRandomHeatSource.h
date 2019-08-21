//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef HACKEDRANDOMHEATSOURCE_H
#define HACKEDRANDOMHEATSOURCE_H

#include "Kernel.h"
//#include "RandomInterface.h"

#include <mutex>

// Forward Declarations
class HackedRandomHeatSource;

template <>
InputParameters validParams<HackedRandomHeatSource>();

class HackedRandomHeatSource : public Kernel //, public RandomInterface
{
public:
  HackedRandomHeatSource(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  void setRandomData();

  Real _specific_heat;
  Real _density;
  Real _delta_T_max;
  Real _energy;
  std::vector<Real> _point2;
  Real _time;

  static Real _t_now;
  static Real _dt_now;
  static std::vector<Real> _point;

  // static std::mutex _lock;

  Real _b;

  std::ofstream fout;
};

#endif
