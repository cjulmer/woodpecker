#include "EventPlacerRandom.h"

#include "MooseRandom.h"

registerMooseObject("woodpeckerApp", EventPlacerRandom);

template <>
InputParameters
validParams<EventPlacerRandom>()
{
  InputParameters params = validParams<EventPlacerBase>();
  params.addParam<int>("seed", 3, "The random number seed.");
  params.addParam<Real>("buffer_dimension", 0.0, "Size of outside border with no events.");
  return params;
}

EventPlacerRandom::EventPlacerRandom(const InputParameters & parameters)
  : EventPlacerBase(parameters),
    _random(declareRestartableData<MooseRandom>("random_generator")),
    _seed(getParam<int>("seed")),
    _inactive(getParam<Real>("buffer_dimension"))
{
  // Seed the random number generator
  _random.seed(0, _seed);
  _random.saveState();
  _t_old = 0.0;

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

  // Reset debug file
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
  {
    _debug.open("Placer.txt", std::ofstream::out);
    _debug.close();
  }
}

void
EventPlacerRandom::execute()
{
  if (_t <= _t_old)
  {
    _random.restoreState();
  }
  _t_old = _t;
  _random.saveState();

  /*
  _event_times = _timer.getActiveEvents();
  _event_locations.resize(0);
  for (auto event = 0; event < _event_times.size(); ++event)
  {
    _event_locations.push_back(getNewPoint());
  }

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
  {
    _debug.open("Placer.txt", std::ofstream::out | std::ofstream::app);
    _debug << "     Placer: execute " << _t << " " << _dt << "\n";
    for (auto event = 0; event < _event_times.size(); ++event)
    {
      _debug << "          " << _event_times[event];
      for (auto coord = 0; coord < _event_locations[event].size(); ++coord)
      {
        _debug << " " << _event_locations[event][coord];
      }
      _debug << "\n";
    }
    _debug.close();
  }
  */
}

std::vector<std::vector<Real>>
EventPlacerRandom::getEventLocations() const
{
  return _event_locations;
}

std::vector<Real>
EventPlacerRandom::getNewPoint() const
{
  std::vector<Real> point;
  point.push_back(_x0 + _random.rand(0) * _x_size);
  point.push_back(_y0 + _random.rand(0) * _y_size);
  point.push_back(_z0 + _random.rand(0) * _z_size);
  return point;
}

std::vector<Real>
EventPlacerRandom::getPoint() const
{
  std::vector<Real> point;
  point.push_back(_x0 + _random.rand(0) * _x_size);
  point.push_back(_y0 + _random.rand(0) * _y_size);
  point.push_back(_z0 + _random.rand(0) * _z_size);
  return point;
}

/*
void
EventPlacerRandom::initialize()
{
  _debug.open("Event.txt", std::ofstream::out | std::ofstream::app);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  _debug << "     Placer: initialize " << rank << " " << _t << " " << _dt << "\n";
  _debug.close();
}

void
EventPlacerRandom::finalize()
{
  _debug.open("Event.txt", std::ofstream::out | std::ofstream::app);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  _debug << "     Placer: finalize " << rank << " " << _t << " " << _dt << "\n";
  _debug.close();
}
*/
