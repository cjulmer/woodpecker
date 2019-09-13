#include "EventTimerRandom.h"

registerMooseObject("woodpeckerApp", EventTimerRandom);

template <>
InputParameters
validParams<EventTimerRandom>()
{
  InputParameters params = validParams<EventTimerBase>();
  return params;
}

EventTimerRandom::EventTimerRandom(const InputParameters & parameters)
  : EventTimerBase(parameters)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  _debug.open("Event.txt", std::ofstream::out);
  _debug << rank << " The object has been created.\n";
  _debug.close();
}

EventTimerRandom::~EventTimerRandom()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  _debug.open("Event.txt", std::ofstream::out | std::ofstream::app);
  _debug << rank << " The object has been deleted.\n";
  _debug.close();
}

void
EventTimerRandom::execute()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  _debug.open("Event.txt", std::ofstream::out | std::ofstream::app);
  _debug << "     " << rank << " " << _random.rand(0);
  if (rank == 0)
  {
    _debug << "     " << _random.rand(0);
  }
  _debug << "     " << _t << "     " << _dt << "\n";
  _debug.close();
}

void
EventTimerRandom::setEvents()
{
  return;
}
