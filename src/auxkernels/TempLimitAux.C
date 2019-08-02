//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TempLimitAux.h"

registerMooseObject("woodpeckerApp", TempLimitAux);

template <>
InputParameters
validParams<TempLimitAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("temperature", "The temperature variable");
  params.addRequiredParam<Real>("temp_limit", "The limit of the temperature");
  return params;
}

TempLimitAux::TempLimitAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _coupled_val(coupledValue("temperature")),
    _limit(getParam<Real>("temp_limit"))
{
}

/**
 * Auxiliary Kernels override computeValue() instead of computeQpResidual().  Aux Variables
 * are calculated either one per elemenet or one per node depending on whether we declare
 * them as "Elemental (Constant Monomial)" or "Nodal (First Lagrange)".  No changes to the
 * source are necessary to switch from one type or the other.
 */
Real
TempLimitAux::computeValue()
{
  if (_coupled_val[_qp] > _limit)
    return _limit;
  else
    return _coupled_val[_qp];
}
