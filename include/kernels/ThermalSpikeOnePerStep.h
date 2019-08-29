#pragma once

#include "Kernel.h"

// Forward Declarations
class ThermalSpikeOnePerStep;

template <>
InputParameters validParams<ThermalSpikeOnePerStep>();

class ThermalSpikeOnePerStep : public Kernel
{
public:
  ThermalSpikeOnePerStep(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  void setRandomData();

  Real _specific_heat;
  Real _density;
  Real _delta_T_max;
  Real _energy;
  std::vector<Real> _point2;
  Real _time;

  static Real _t_now;
  static Real _dt_now;
  static std::vector<Real> _point;

  Real _b;

  std::ofstream fout;
};
