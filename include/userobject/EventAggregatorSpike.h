#pragma once

// Moose
#include "EventAggregatorBase.h"

// Forward declaration
class EventAggregatorSpike;

template <>
InputParameters validParams<EventAggregatorSpike>();

class EventAggregatorSpike : public EventAggregatorBase
{
public:
  EventAggregatorSpike(const InputParameters & parameters);

  virtual void initialize() override{};
  virtual void execute() override;
  virtual void finalize() override{};

protected:
  std::vector<Real> _spike_times;
  std::vector<std::vector<Real>> _spike_locations;
  std::vector<Real> _spike_energies;
  std::ofstream _debug;
};
