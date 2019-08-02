//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef RANDOMHEATSOURCE_H
#define RANDOMHEATSOURCE_H

#include "DiracKernel.h"
#include "RandomInterface.h"

// Forward Declarations
class RandomHeatSource;

template <>
InputParameters validParams<RandomHeatSource>();

class RandomHeatSource : public DiracKernel,
                         public RandomInterface
{
public:
  RandomHeatSource(const InputParameters & parameters);
protected:
  virtual void addPoints() override;
  virtual Real computeQpResidual() override;

  Real _specific_heat;
  Real _density;
  Real _delta_T_max;
  Real _energy;
  std::vector<Real> _point;
  Real _time;

  Real _b;

  Real r1;
  Real r2;
  Real r3;
  std::fstream fout;
};

#endif
