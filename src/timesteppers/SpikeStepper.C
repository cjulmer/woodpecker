#include "SpikeStepper.h"

#include "SpikeTimer.h"

registerMooseObject("woodpeckerApp", SpikeStepper);

template <>
InputParameters
validParams<SpikeStepper>()
{
  InputParameters params = validParams<TimeStepper>();
  params.addRequiredParam<Real>("dt", "The initial timestep");
  // params.addParam<std::vector<Real>>("time_t", "The values of t");
  // params.addParam<std::vector<Real>>("time_dt", "The values of dt");
  params.addParam<Real>("growth_factor", 2.0, "Factor to apply to timestep if easy convergence");
  params.addParam<Real>("cutback_factor", 0.5, "Factor to apply to timestep if solution failed");
  params.addParam<Real>("spike_dt", 1.0e-5, "Timestep for thermal spike");
  params.addRequiredParam<UserObjectName>("spike_timer", "Thermal spike timer User Object");
  // params.declareControllable("growth_factor cutback_factor");
  return params;
}

SpikeStepper::SpikeStepper(const InputParameters & parameters)
  : TimeStepper(parameters),
    UserObjectInterface(this),
    _input_dt(getParam<Real>("dt")),
    _growth_factor(getParam<Real>("growth_factor")),
    _cutback_factor(getParam<Real>("cutback_factor")),
    _spike_dt(getParam<Real>("spike_dt")),
    _spike_timer(getUserObject<SpikeTimer>("spike_timer")),
    _failed_dt(false)
{
  MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
  if (_rank == 0)
  {
    _debug.open("SpikeStepper.txt");
    _debug.close();
  }
}

Real
SpikeStepper::computeInitialDT()
{
  Real new_dt = _input_dt;

  Real next_spike = _spike_timer.getNextSpike(_time);

  if (next_spike < (_time + new_dt))
  {
    if (next_spike < (_time + _spike_dt))
    {
      new_dt = _spike_dt;
    }
    else
    {
      new_dt = next_spike - _time;
    }
  }

  return new_dt;
}

Real
SpikeStepper::computeDT()
{
  Real new_dt;
  if (_failed_dt)
  {
    _failed_dt = false;
    new_dt = _dt;
  }
  else
  {
    new_dt = _growth_factor * _dt;
  }

  /*if (_rank == 0)
  {
    _debug.open("SpikeStepper.txt" | std::ofstream::app);
    _debug << "t: " << _time << "     dt: " << _dt << "     new dt: " << new_dt << "\n";
    _debug.close();
  }*/

  Real next_spike = _spike_timer.getNextSpike(_time);

  if (next_spike < (_time + new_dt))
  {
    if (next_spike < (_time + _spike_dt))
    {
      new_dt = _spike_dt;
    }
    else
    {
      new_dt = next_spike - _time;
    }
  }

  return new_dt;
}

Real
SpikeStepper::computeFailedDT()
{
  _failed_dt = true;
  _console << "\nTEST:     t: " /*<< _t*/ << "     dt: " << _dt << " failed\n";

  if (_dt <= _dt_min)
    mooseError("Solve failed and timestep already at or below dtmin, cannot continue!");

  // cut the time step in a half
  if (_cutback_factor * _dt >= _dt_min)
    return _cutback_factor * _dt;
  else // (0.5 * _current_dt < _dt_min)
    return _dt_min;
}
