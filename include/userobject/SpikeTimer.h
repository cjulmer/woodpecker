#pragma once

// Moose
#include "GeneralUserObject.h"
#include "SphericalSpike.h"

// Forward declaration
class SpikeTimer;
class MooseRandom;
class MooseMesh;
class SphericalSpike;
// class deque;

template <>
InputParameters validParams<SpikeTimer>();

class SpikeTimer : public GeneralUserObject
{
public:
  SpikeTimer(const InputParameters & parameters);

  virtual void initialize() override{};
  virtual void finalize() override{};
  virtual void execute() override;

  std::vector<SphericalSpike> getActiveSpikes() const;
  Real getNextSpike() const;

protected:
  SphericalSpike getSpike();
  Real getInterval();
  Real getEnergy();
  Point getPoint();
  //std::vector<SphericalSpike> getActiveSpikes();

  MooseRandom & _random;
  MooseMesh & _mesh;
  Real _first_spike;
  Real _spike_rate;
  int _seed;
  Real _inactive;

  std::deque<SphericalSpike> _spikes;
  Real _x_size, _y_size, _z_size;
  Real _x0, _y0, _z0;

  int _rank;
  std::ofstream _debug;
};
