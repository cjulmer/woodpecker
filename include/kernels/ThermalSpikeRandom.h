#pragma once

#include "Kernel.h"

#include <deque>

// Forward Declarations
class ThermalSpikeRandom;

template <>
InputParameters validParams<ThermalSpikeRandom>();

class ThermalSpikeRandom : public Kernel
{
public:
  ThermalSpikeRandom(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  void setRandomData();
  Real getSpikeInterval();

  Real _specific_heat;
  Real _density;
  Real _delta_T_max;
  Real _energy;
  Real _spike_rate;
  Real _initiation_time;

  Real _t_now;
  Real _dt_now;
  std::vector<Real> _point;
  std::vector<Real> _points;
  std::deque<Real> _spikes;
  int _dim;

  Real _b;

  std::ofstream fout;
};
