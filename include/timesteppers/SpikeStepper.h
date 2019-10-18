#pragma once

#include "TimeStepper.h"
#include "UserObjectInterface.h"

class SpikeStepper;
class SpikeTimer;

template <>
InputParameters validParams<SpikeStepper>();

class SpikeStepper : public TimeStepper, public UserObjectInterface
{
public:
  SpikeStepper(const InputParameters & parameters);

  // virtual void init() override;
  // virtual void preExecute() override;

  // virtual void rejectStep() override;
  // virtual void acceptStep() override;

  // virtual bool constrainStep(Real & dt) override;

protected:
  virtual Real computeInitialDT() override;
  virtual Real computeDT() override;
  virtual Real computeFailedDT() override;
  // virtual bool converged() const override;

  Real _input_dt;
  Real _growth_factor;
  Real _cutback_factor;
  Real _spike_dt;
  /// grow the timestep by this factor
  // const Real & _growth_factor;
  /// cut the timestep by by this factor
  // const Real & _cutback_factor;

  const SpikeTimer & _spike_timer;
  bool _failed_dt;

  std::ofstream _debug;
  int _rank;
};
