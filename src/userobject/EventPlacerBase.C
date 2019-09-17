#include "EventPlacerBase.h"

#include "MooseMesh.h"
#include "EventTimerBase.h"

template <>
InputParameters
validParams<EventPlacerBase>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addRequiredParam<UserObjectName>("event_timer", "The name of the event timer User Object.");

  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_BEGIN;

  return params;
}

EventPlacerBase::EventPlacerBase(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _mesh(_fe_problem.mesh()),
  _timer(getUserObject<EventTimerBase>("event_timer"))
{
}
