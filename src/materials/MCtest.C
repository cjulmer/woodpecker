//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MCtest.h"

registerMooseObject("woodpeckerApp", MCtest);

template <>
InputParameters
validParams<MCtest>()
{
  InputParameters params = validParams<Material>();

  // Add a parameter to get the radius of the spheres in the column (used later to interpolate
  // permeability).
  params.addParam<Real>("sphere_radius",
                        1.0,
                        "The radius of the steel spheres that are packed in the "
                        "column.  Used to interpolate _permeability.");

  return params;
}

MCtest::MCtest(const InputParameters & parameters)
  : Material(parameters),

    // Get the one parameter from the input file
    _sphere_radius(getParam<Real>("sphere_radius")),

    // Declare two material properties.  This returns references that we
    // hold onto as member variables
    _permeability(declareProperty<Real>("permeability")),
    _viscosity(declareProperty<Real>("viscosity"))
{
  setRandomResetFrequency(EXEC_TIMESTEP_BEGIN);
}

Real
MCtest::getRandom()
{
  random_number = getRandomReal();
  return random_number;
}

void
MCtest::computeQpProperties()
{
  return;
}
