#pragma once

// Moose
#include "GeneralUserObject.h"

// Forward declaration
class EventAggregatorBase;

class EventTimerBase;
class EventPlacerBase;

template <>
InputParameters validParams<EventAggregatorBase>();

class EventAggregatorBase : public GeneralUserObject
{
public:
  EventAggregatorBase(const InputParameters & parameters);

protected:
  const EventTimerBase & _timer;
  const EventPlacerBase & _placer;

  std::ofstream _debug;
};
