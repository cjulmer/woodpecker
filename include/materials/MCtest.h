//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef MCTEST_H
#define MCTEST_H

#include "Material.h"

class MCtest;

template <>
InputParameters validParams<MCtest>();

/**
 * Material objects inherit from Material and override computeQpProperties.
 *
 * Their job is to declare properties for use by other objects in the
 * calculation such as Kernels and BoundaryConditions.
 */
class MCtest : public Material
{
public:
  MCtest(const InputParameters & parameters);
  Real getRandom();

protected:
  /**
   * Necessary override.  This is where the values of the properties
   * are computed.
   */
  virtual void computeQpProperties() override;

  Real _sphere_radius;

  /// The permeability (K)
  MaterialProperty<Real> & _permeability;

  /// The viscosity of the fluid (mu)
  MaterialProperty<Real> & _viscosity;

  Real random_number;
};

#endif // MCtest_H
