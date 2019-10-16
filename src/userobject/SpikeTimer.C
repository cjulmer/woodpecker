#include "SpikeTimer.h"

#include <deque>
#include "SphericalSpike.h"

registerMooseObject("woodpeckerApp", SpikeTimer);

template <>
InputParameters
validParams<SpikeTimer>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addParam<Real>("start_time", 0.0, "The time to start the thermal spikes.");
  params.addRequiredParam<Real>("spike_rate", "Average rate of thermal spikes.");
  params.addParam<int>("seed", 3, "The random number seed.");
  params.addParam<Real>("buffer_dimension", 0.0, "Size of outside border with no events.");
  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_BEGIN;
  return params;
}

SpikeTimer::SpikeTimer(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _random(declareRestartableData<MooseRandom>("random_generator")),
    _mesh(_fe_problem.mesh()),
    _first_spike(getParam<Real>("start_time")),
    _spike_rate(getParam<Real>("spike_rate")),
    _seed(getParam<int>("seed")),
    _inactive(getParam<Real>("buffer_dimension"))
{

  // Reset debug file
  MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
  if (_rank == 0)
  {
    _debug.open("SpikeTimer.txt", std::ofstream::out);
    _debug.close();
  }

  // Seed the random number generator
  _random.seed(0, _seed);

  // Get mesh size properties
  _x_size = _mesh.dimensionWidth(0);
  _y_size = _mesh.dimensionWidth(1);
  _z_size = _mesh.dimensionWidth(2);
  _x0 = _mesh.getMinInDimension(0);
  _y0 = _mesh.getMinInDimension(1);
  _z0 = _mesh.getMinInDimension(2);

  // Modify domain size for inactive region
  if (_z_size > 0.0)
  {
    _z_size -= 2.0 * _inactive;
    _z0 += _inactive;
  }
  if (_y_size > 0.0)
  {
    _y_size -= 2.0 * _inactive;
    _y0 += _inactive;
  }
  _x_size -= 2.0 * _inactive;
  _x0 += _inactive;

  // Set the first event
  _spikes.push_back(getSpike());

}

void
SpikeTimer::execute()
{
  //_debug.open("SpikeTimer.txt", std::ofstream::out | std::ofstream::app);
  //_debug << "     rank " << _rank << "     " << _dt << "     " << _t << "\n";

  while (_spikes.front().getTime() < (_t - _dt))
  {
    _spikes.pop_front();
  }

  while (_spikes.back().getTime() < _t)
  {
    //_debug << "          rank " << _rank << " last " << _spikes.back().getTime() << "\n";
    _spikes.push_back(getSpike());
  }

  std::vector<SphericalSpike> temp = getActiveSpikes();
  //_debug << "          rank " << _rank << " last " << _spikes.back().getTime() << "\n";
  //_debug.close();
  /*
  _spike_times = _timer.getActiveEvents();
  _spike_locations.resize(0);
  _spike_energies.resize(0);
  for (auto i = 0; i < _spike_times.size(); ++i)
  {
    _spike_locations.push_back(_placer.getPoint());
    _spike_energies.push_back(20.0e3);
  }

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
  {
    _debug.open("Aggregator.txt", std::ofstream::out | std::ofstream::app);
    _debug << "     Aggregator: execute " << _t << " " << _dt << "\n";
    for (auto i = 0; i < _spike_times.size(); ++i)
    {
      _debug << "          " << _spike_times[i];
      for (auto j = 0; j < _spike_locations[i].size(); ++j)
      {
        _debug << " " << _spike_locations[i][j];
      }
      _debug << " " << _spike_energies[i] << "\n";
    }
    _debug.close();
  }
  */
  /*
  _event_times = _timer.getActiveEvents();
  _event_locations.resize(0);
  for (auto event = 0; event < _event_times.size(); ++event)
  {
    _event_locations.push_back(getNewPoint());
  }
  */
}

SphericalSpike
SpikeTimer::getSpike()
{
  Real spikeTime;

  // first spike time
  if (_spikes.empty())
  {
    if (_first_spike < (_t - _dt))
    {
      // something has gone wrong!
      _debug << "ERROR: trying to create initial spike at time " << _first_spike
             << " when timestep ranges from " << (_t - _dt) << " to " << _t << " !\n";
    }
    else
    {
      spikeTime = _first_spike;
    }
  }
  // other spike times
  else
  {
    spikeTime = getInterval() + _spikes.back().getTime();
  }

  Real spikeEnergy = getEnergy();
  Point spikePoint = getPoint();

  if (_rank == 0)
  {
    _debug.open("SpikeTimer.txt", std::ofstream::out | std::ofstream::app);
    _debug << spikeTime << "     " << spikeEnergy << "     " << spikePoint(0) << "     ";
    _debug << spikePoint(1) << "     " << spikePoint(2) << "\n";
    _debug.close();
  }

  return SphericalSpike(spikeTime, spikeEnergy, spikePoint);
}

Real
SpikeTimer::getInterval()
{
  return -std::log(1.0 - _random.rand(0)) / _spike_rate;
}

Real
SpikeTimer::getEnergy()
{
  return 20.0;
}

Point
SpikeTimer::getPoint()
{
  return Point(_x0 + _random.rand(0) * _x_size,
               _y0 + _random.rand(0) * _y_size,
               _z0 + _random.rand(0) * _z_size);
}

std::vector<SphericalSpike>
SpikeTimer::getActiveSpikes() const
{
  std::vector<SphericalSpike> spikes;
  for (auto i = 0; i < _spikes.size(); ++i)
  {
    if (_spikes[i].getTime() < _t && _spikes[i].getTime() >= (_t - _dt))
    {
      spikes.push_back(_spikes[i]);
    }
  }
  if (_rank == 0)
  {
    //_debug.open("SpikeTimer.txt", std::ofstream::out | std::ofstream::app);
    for (auto i = 0; i < spikes.size(); ++i)
    {
      //_debug << "     " << spikes[i].getTime() << "     " << spikes[i].getEnergy() << "     ";
      //_debug << spikes[i].getLocation()(0) << "     ";
      //_debug << spikes[i].getLocation()(1) << "     " << spikes[i].getLocation()(2) << "\n";
    }
    //_debug.close();
  }
  return spikes;
}

Real
SpikeTimer::getNextSpike() const
{
  return _spikes.back().getTime();
}
