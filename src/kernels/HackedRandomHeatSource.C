//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "HackedRandomHeatSource.h"

// static initialization
Real HackedRandomHeatSource::_t_now = -1.0;
Real HackedRandomHeatSource::_dt_now = -1.0;
std::vector<Real> HackedRandomHeatSource::_point;
// std::mutex HackedRandomHeatSource::_lock;

registerMooseObject("woodpeckerApp", HackedRandomHeatSource);

template <>
InputParameters
validParams<HackedRandomHeatSource>()
{
  InputParameters params = validParams<Kernel>();
  // params += validParams<RandomInterface>();

  params.addRequiredParam<Real>("specific_heat", "Specific heat of the material");
  params.addRequiredParam<Real>("density_name", "Density of the material");
  params.addRequiredParam<Real>("delta_T_max", "Maximum temperature increase due to thermal spike");
  params.addRequiredParam<Real>("energy", "Energy of thermal spike");
  params.addRequiredParam<std::vector<Real>>("point", "The x,y,z coordinates of the point");
  params.addRequiredParam<Real>("time", "Time of thermal spike initiation");
  return params;
}

HackedRandomHeatSource::HackedRandomHeatSource(const InputParameters & parameters)
  : Kernel(parameters),
    // RandomInterface(parameters,
    //                *parameters.getCheckedPointerParam<FEProblemBase *>("_fe_problem_base"),
    //                parameters.get<THREAD_ID>("_tid"),
    //                false),
    _specific_heat(getParam<Real>("specific_heat")),
    _density(getParam<Real>("density_name")),
    _delta_T_max(getParam<Real>("delta_T_max")),
    _energy(getParam<Real>("energy")),
    _point2(getParam<std::vector<Real>>("point")),
    _time(getParam<Real>("time"))
{
  _point.resize(2);
  fout.open("test.txt");
  fout.close();
  setRandomResetFrequency(EXEC_TIMESTEP_BEGIN);
  _b = std::sqrt(_energy / (2 * 3.14159265359 * _density * _specific_heat * _delta_T_max));
}

Real
HackedRandomHeatSource::computeQpResidual()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // fout << "Process " << rank << " before" << "\n";
  // fout << "  t " << _t << " : " << _t_now << "\n";
  // fout << "  dt " << _dt << " : " << _dt_now << "\n";

  if (_t != _t_now || _dt != _dt_now)
  {
    fout.open("test.txt", std::ofstream::out | std::ofstream::app);
    if (size > 1)
    {
      if (rank == 0)
      {
        setRandomData();
        MPI_Bcast(_point.data(), _point.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      }
      else
      {
        MPI_Bcast(_point.data(), _point.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      }
    }
    else
    {
      setRandomData();
    }
    _t_now = _t;
    _dt_now = _dt;
    // MPI_Barrier(MPI_COMM_WORLD);
    fout << "Process " << rank << " after"
         << "\n";
    fout << "  t " << _t << " : " << _t_now << "\n";
    fout << "  dt " << _dt << " : " << _dt_now << "\n";
    for (auto point : _point)
      fout << "    " << point << "\n";
    fout.close();
  }

  /*
  fout.open("test.txt", std::ofstream::out | std::ofstream::app);
  fout << _t << " " << _t_now << "\n";
  fout << _dt << " " << _dt_now << "\n";
  if (_t != _t_now || _dt != _dt_now)
  {
    std::lock_guard<std::mutex> lock(_lock);
    fout << "     " << _t << " " << _t_now << "\n";
    fout << "     " << _dt << " " << _dt_now << "\n";
    if (_t != _t_now || _dt != _dt_now)
    {
      setRandomData();
      _t_now = _t;
      _dt_now = _dt;
    }
    fout << "          " << _t << " " << _t_now << "\n";
    fout << "          " << _dt << " " << _dt_now << "\n";
  }
  fout.close();
  */

  // if (_time >= (_t - _dt) && _time < _t)
  //{
  Point p = _q_point[_qp];
  Real x = _q_point[_qp](0);
  Real y = _q_point[_qp](1);
  Real factor = _specific_heat * _density * _delta_T_max *
                std::exp(-((_point[0] - x) * (_point[0] - x) + (_point[1] - y) * (_point[1] - y)) /
                         (2 * _b * _b));
  return _test[_i][_qp] * -factor / _dt;
  //}
  // else
  //{
  //  return 0;
  //}
}

void
HackedRandomHeatSource::setRandomData()
{
  // fout.open("test.txt", std::ofstream::out | std::ofstream::app);
  // fout << _t << " " << _dt << "\n";

  _point.resize(0);
  for (auto i = 1; i < 3; ++i)
  {
    Real val = getRandomReal();
    //  fout << "    " << val << "\n";
    val *= 20.0;
    //  fout << "    " << val << "\n";
    _point.push_back(val);
    //  fout << "    " << _point.back() << "\n";
    //  _point.push_back(getRandomReal()*20.0);
  }
  //  for (auto item : _point)
  //{
  // fout << "        " << item << "\n";
  //}
  // fout.close();
  return;
}
