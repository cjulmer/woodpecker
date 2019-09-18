#pragma once

// Moose
#include "EventPlacerBase.h"

// Forward declaration
class EventPlacerRandom;
class MooseRandom;

template <>
InputParameters validParams<EventPlacerBase>();

class EventPlacerRandom : public EventPlacerBase
{
public:
  EventPlacerRandom(const InputParameters & parameters);

  virtual void initialize() override{};
  virtual void execute() override;
  virtual void finalize() override{};
  virtual std::vector<std::vector<Real>> getEventLocations() const override;
  virtual std::vector<Real> getPoint() const override;

protected:
  std::vector<Real> getNewPoint() const;

  MooseRandom & _random;
  int _seed;
  Real _inactive;

  std::vector<Real> _event_times;
  std::vector<std::vector<Real>> _event_locations;
  Real _x_size, _y_size, _z_size;
  Real _x0, _y0, _z0;
  Real _t_old;
};
