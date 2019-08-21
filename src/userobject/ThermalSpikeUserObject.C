#include "ThermalSpikeUserObject.h"

registerMooseObject("woodpeckerApp", ThermalSpikeUserObject);

template <>
InputParameters
validParams<ThermalSpikeUserObject>()
{
  InputParameters params = validParams<GeneralUserObject>();
  return params;
}

ThermalSpikeUserObject::ThermalSpikeUserObject(const InputParameters & parameters)
  : GeneralUserObject(parameters) /*,
     RandomInterface(parameters,
                     *parameters.getCheckedPointerParam<FEProblemBase *>("_fe_problem_base"),
                     parameters.get<THREAD_ID>("_tid"),
                     false)*/
{
  fout.open("ThermalSpikeUserObject.txt");
  fout << "The object has been created.\n";
  fout.close();
  // setRandomResetFrequency(EXEC_TIMESTEP_BEGIN);
}

ThermalSpikeUserObject::~ThermalSpikeUserObject()
{
  fout.open("ThermalSpikeUserObject.txt", std::ofstream::out | std::ofstream::app);
  fout << "The object has been deleted.\n";
  fout.close();
}

void
ThermalSpikeUserObject::execute()
{
  fout.open("ThermalSpikeUserObject.txt", std::ofstream::out | std::ofstream::app);
  fout << "     " << _tid << "     " << _t << "     " << _dt << "\n";
  fout.close();
}
