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
  EventTimerBase(const InputParameters & parameters);
  // virtual ~EventTimerBase();

  virtual std::vector<Real> getActiveEvents() const = 0;
  virtual Real getNextEvent() const = 0;

protected:
  MooseRandom & _random;
  Real _first_event;

  std::ofstream _debug;
};
