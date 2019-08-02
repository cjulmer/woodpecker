//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReactionRateAux.h"

registerMooseObject("woodpeckerApp", ReactionRateAux);

template <>
InputParameters
validParams<ReactionRateAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("temperature", "The temperature variable");
  params.addRequiredParam<Real>("reaction_energy", "The energy for reaction... Q-value [eV]");
  params.addParam<Real>("coefficient", 1.0, "A simple coefficient");
  return params;
}

ReactionRateAux::ReactionRateAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _coupled_val(coupledValue("temperature")),
    _energy(getParam<Real>("reaction_energy")),
    _coef(getParam<Real>("coefficient"))
{
}

/**
 * Auxiliary Kernels override computeValue() instead of computeQpResidual().  Aux Variables
 * are calculated either one per elemenet or one per node depending on whether we declare
 * them as "Elemental (Constant Monomial)" or "Nodal (First Lagrange)".  No changes to the
 * source are necessary to switch from one type or the other.
 */
Real
ReactionRateAux::computeValue()
{
  return std::exp(-_energy / 8.617333e-5 / _coupled_val[_qp]) * _coef;
}
