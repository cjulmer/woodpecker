#pragma once

#include "EventTimerBase.h"

#include<deque>

// Forward declaration
class EventTimerRandom;
class MooseMesh;

template <>
InputParameters validParams<EventTimerRandom>();

class EventTimerRandom : public EventTimerBase
{
public:
  EventTimerRandom(const InputParameters & parameters);
  virtual ~EventTimerRandom() {};

  virtual void initialize() override {};
  virtual void finalize() override {};
  virtual void execute() override;

  virtual std::vector<Real> getActiveEvents() const override;
  //std::vector<Real> getActiveEvents(Real, Real);
  //bool isEvent();
  //bool isEvent(Real,Real);
  Real getNextEvent() const override;
  //Real nextEvent(Real,Real);

protected:
  Real nextInterval();

  MooseMesh & _mesh;
  Real _event_rate;
  int _seed;

  std::deque<Real> _events;
};
