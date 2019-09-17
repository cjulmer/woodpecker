#pragma once

// Moose
#include "GeneralUserObject.h"

// Forward declaration
class EventPlacerBase;
class MooseMesh;
class EventTimerBase;

template <>
InputParameters validParams<EventPlacerBase>();

class EventPlacerBase : public GeneralUserObject
{
public:
  EventPlacerBase(const InputParameters & parameters);

  virtual std::vector<std::vector<Real>> getEventLocations() const = 0;

protected:
  MooseMesh & _mesh;
  const EventTimerBase & _timer;

  std::ofstream _debug;
};
