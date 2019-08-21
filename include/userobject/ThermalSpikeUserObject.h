#ifndef THERMALSPIKEUSEROBJECT_H
#define THERMALSPIKEUSEROBJECT_H

// Moose
#include "GeneralUserObject.h"
#include "RandomInterface.h"

// Forward declaration
class ThermalSpikeUserObject;

template <>
InputParameters validParams<ThermalSpikeUserObject>();

class ThermalSpikeUserObject : public GeneralUserObject /*,
                                public RandomInterface*/
{
public:
  ThermalSpikeUserObject(const InputParameters & parameters);
  virtual ~ThermalSpikeUserObject();

  virtual void execute();
  virtual void initialize(){};
  virtual void finalize(){};

protected:
  std::ofstream fout;
};

#endif // THERMALSPIKEUSEROBJECT_H
