#include "EventAggregatorSpike.h"

registerMooseObject("woodpeckerApp", EventAggregatorSpike);

template <>
InputParameters
validParams<EventAggregatorSpike>()
{
  InputParameters params = validParams<EventAggregatorBase>();
  return params;
}

EventAggregatorSpike::EventAggregatorSpike(const InputParameters & parameters)
  : EventAggregatorBase(parameters)
{
  // Reset debug file
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
  {
    _debug.open("Aggregator.txt", std::ofstream::out);
    _debug.close();
  }
}

void
EventAggregatorSpike::execute()
{
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

  /*
  _event_times = _timer.getActiveEvents();
  _event_locations.resize(0);
  for (auto event = 0; event < _event_times.size(); ++event)
  {
    _event_locations.push_back(getNewPoint());
  }
  */
}
