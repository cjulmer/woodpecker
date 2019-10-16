#include "EventAggregatorBase.h"

#include "EventTimerBase.h"
#include "EventPlacerBase.h"


template <>
InputParameters
validParams<EventAggregatorBase>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addRequiredParam<UserObjectName>("event_timer",
                                          "The name of the event timer User Object.");
  params.addRequiredParam<UserObjectName>("event_placer",
                                          "The name of the event placer User Object.");

  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_BEGIN;

  return params;
}

EventAggregatorBase::EventAggregatorBase(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _timer(getUserObject<EventTimerBase>("event_timer")),
    _placer(getUserObject<EventPlacerBase>("event_placer"))
{
}
