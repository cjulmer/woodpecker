//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef INSTANTPOINTSOURCE_H
#define INSTANTPOINTSOURCE_H

// Moose Includes
#include "DiracKernel.h"

// Forward Declarations
class InstantPointSource;

template <>
InputParameters validParams<InstantPointSource>();

/**
 * TOOD
 */
class InstantPointSource : public DiracKernel
{
public:
  InstantPointSource(const InputParameters & parameters);

  virtual void addPoints() override;

protected:
  virtual Real computeQpResidual() override;

  const Real & _value;
  std::vector<Real> _point_param;
  Point _p;
  Real _time;
};

#endif // INSTANTPOINTSOURCE_H
