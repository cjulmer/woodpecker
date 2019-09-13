#pragma once

#include "EventTimerBase.h"

// Forward declaration
class EventTimerRandom;

template <>
InputParameters validParams<EventTimerRandom>();

class EventTimerRandom : public EventTimerBase
{
public:
  EventTimerRandom (const InputParameters & parameters);
  virtual ~EventTimerRandom();

  virtual void initialize () override {};
  virtual void finalize () override {};
  virtual void execute () override;

protected:
  virtual void setEvents() override;

};
