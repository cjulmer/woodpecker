#include "EventTimerBase.h"

#include "MooseRandom.h"

//registerMooseObject("woodpeckerApp", EventTimerBase);

template <>
InputParameters
validParams<EventTimerBase>()
{
  InputParameters params = validParams<GeneralUserObject>();

  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_BEGIN;

  return params;
}

EventTimerBase::EventTimerBase(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _random(declareRestartableData<MooseRandom>("random_generator"))
{
}
