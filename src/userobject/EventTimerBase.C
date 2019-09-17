#include "EventTimerBase.h"

#include "MooseRandom.h"

// registerMooseObject("woodpeckerApp", EventTimerBase);

template <>
InputParameters
validParams<EventTimerBase>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addParam<Real>("start_time", 0.0, "The time to start the events.");

  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_BEGIN;

  return params;
}

EventTimerBase::EventTimerBase(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _random(declareRestartableData<MooseRandom>("random_generator")),
  _first_event(getParam<Real>("start_time"))
{
}
