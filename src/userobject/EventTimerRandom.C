#include "EventTimerRandom.h"
#include "MooseMesh.h"

registerMooseObject("woodpeckerApp", EventTimerRandom);

template <>
InputParameters
validParams<EventTimerRandom>()
{
  InputParameters params = validParams<EventTimerBase>();
  params.addRequiredParam<Real>("event_rate", "Average rate of events.");
  params.addParam<int>("seed", 3, "The random number seed.");
  return params;
}

EventTimerRandom::EventTimerRandom(const InputParameters & parameters)
  : EventTimerBase(parameters),
    _mesh(_fe_problem.mesh()),
    _event_rate(getParam<Real>("event_rate")),
    _seed(getParam<int>("seed"))
{
  // Seed the random number generator
  _random.seed(0, _seed);

  // Initialize the events queue
  _events.resize(0);
  _events.push_back(_first_event);

  // Multiply event rate by domain area/volume
  for (auto i = 0; i < 3; ++i)
  {
    Real size = _mesh.dimensionWidth(i);
    if (size > 0.0)
    {
      _event_rate *= size;
    }
  }
}

void
EventTimerRandom::execute()
{
  //_debug.open("Event.txt", std::ofstream::out | std::ofstream::app);
  // int rank;
  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  //_debug << "     Timer: execute " << rank << " " << _t << " " << _dt << "\n";

  // delete events that are no longer used
  while (_events.front() < (_t - _dt))
  {
    _events.pop_front();
  }

  // add new events for time step
  while (_events.back() < _t)
  {
    _events.push_back(_events.back() + nextInterval());
  }

  //_debug.close();
}

/*
void
EventTimerRandom::initialize()
{
  _debug.open("Event.txt", std::ofstream::out | std::ofstream::app);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  _debug << "     Timer: initialize " << rank << " " << _t << " " << _dt << "\n";
  _debug.close();
}

void
EventTimerRandom::finalize()
{
  _debug.open("Event.txt", std::ofstream::out | std::ofstream::app);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  _debug << "     Timer: finalize " << rank << " " << _t << " " << _dt << "\n";
  _debug.close();
}
*/

std::vector<Real>
EventTimerRandom::getActiveEvents() const
{
  std::vector<Real> activeEvents;
  for (auto event : _events)
  {
    if (event < _t && event >= (_t - _dt))
    {
      activeEvents.push_back(event);
    }
  }
  return activeEvents;
}

/*
std::vector<Real>
EventTimerRandom::getActiveEvents(Real t, Real dt)
{
  std::vector<Real> activeEvents;
  for (auto event : _events)
  {
    if (event < t && event >= (t - dt))
    {
      activeEvents.push_back(event);
    }
  }
  return activeEvents;
}

bool
EventTimerRandom::isEvent()
{
  for (auto event : _events)
  {
    if (event < _t && event >= (_t - _dt))
    {
      return true;
    }
  }
  return false;
}

bool
EventTimerRandom::isEvent(Real t, Real dt)
{
  for (auto event : _events)
  {
    if (event < t && event >= (t - dt))
    {
      return true;
    }
  }
  return false;
}
*/

Real
EventTimerRandom::getNextEvent() const
{
  return _events.front();
}

Real
EventTimerRandom::nextInterval()
{
  return -std::log(1.0 - _random.rand(0)) / _event_rate;
}
