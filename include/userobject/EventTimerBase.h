#pragma once

// Moose
#include "GeneralUserObject.h"

// Forward declaration
class EventTimerBase;
class MooseRandom;

template <>
InputParameters validParams<EventTimerBase>();

class EventTimerBase : public GeneralUserObject
{
public:
  EventTimerBase (const InputParameters & parameters);
  //virtual ~EventTimerBase();

protected:
  virtual void setEvents() = 0;

  MooseRandom & _random;
  std::vector<Real> _events;

  std::ofstream _debug;
};
