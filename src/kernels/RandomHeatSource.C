//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RandomHeatSource.h"

registerMooseObject("woodpeckerApp", RandomHeatSource);

template <>
InputParameters
validParams<RandomHeatSource>()
{
  InputParameters params = validParams<DiracKernel>();
  params += validParams<RandomInterface>();

  params.addRequiredParam<Real>("specific_heat", "Specific heat of the material");
  params.addRequiredParam<Real>("density_name", "Density of the material");
  params.addRequiredParam<Real>("delta_T_max", "Maximum temperature increase due to thermal spike");
  params.addRequiredParam<Real>("energy", "Energy of thermal spike");
  params.addRequiredParam<std::vector<Real>>("point", "The x,y,z coordinates of the point");
  params.addRequiredParam<Real>("time", "Time of thermal spike initiation");
  return params;
}

RandomHeatSource::RandomHeatSource(const InputParameters & parameters)
  : DiracKernel(parameters),
    RandomInterface(parameters,
                    *parameters.getCheckedPointerParam<FEProblemBase *>("_fe_problem_base"),
                    parameters.get<THREAD_ID>("_tid"),
                    false),
    _specific_heat(getParam<Real>("specific_heat")),
    _density(getParam<Real>("density_name")),
    _delta_T_max(getParam<Real>("delta_T_max")),
    _energy(getParam<Real>("energy")),
    _point(getParam<std::vector<Real>>("point")),
    _time(getParam<Real>("time"))
{
  fout.open("test.txt");
  fout.close();
  setRandomResetFrequency(EXEC_TIMESTEP_BEGIN);
  _b = std::sqrt(_energy / (2*3.14159265359*_density*_specific_heat*_delta_T_max));
}

void
RandomHeatSource::addPoints()
{
  r1 = getRandomReal();
  r2 = getRandomReal();
  r2 = getRandomReal();
  fout.open("test.txt", std::ofstream::out | std::ofstream::app);
  fout << _t << " " << r1 << " " << r2 << " " << r2 << " " << getRandomReal() << "\n";
  fout.close();
  return;
}

Real
RandomHeatSource::computeQpResidual()
{
  if (_time >= (_t - _dt) && _time < _t)
  {
    Point p = _q_point[_qp];
    Real x = _q_point[_qp](0);
    Real y = _q_point[_qp](1);
    Real factor = _specific_heat*_density*_delta_T_max * std::exp(-((_point[0]-x)*(_point[0]-x)+(_point[1]-y)*(_point[1]-y))/(2*_b*_b));
    return _test[_i][_qp] * -factor / _dt;
  }
  else
  {
    return 0;
  }
}
