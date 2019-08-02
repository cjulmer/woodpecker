//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef GAUSSIANHEATSOURCE_H
#define GAUSSIANHEATSOURCE_H

#include "Kernel.h"

// Forward Declarations
class GaussianHeatSource;

template <>
InputParameters validParams<GaussianHeatSource>();

class GaussianHeatSource : public Kernel
{
public:
  GaussianHeatSource(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  Real _specific_heat;
  Real _density;
  Real _delta_T_max;
  Real _energy;
  std::vector<Real> _point;
  Real _time;

  Real _b;
};

#endif
