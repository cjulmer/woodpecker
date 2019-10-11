#pragma once

// I have no idea which header files are needed for libmesh::Point
#include "GeneralUserObject.h"
//#include "Moose.h"
//#include "Point.h"

class SphericalSpike
{
public:
  SphericalSpike (Real time, Real energy, Real x, Real y = 0.0, Real z = 0.0);
  SphericalSpike (Real time, Real energy, Point location);

  Real getTime() const;
  Real getEnergy() const;
  Point getLocation() const;

protected:
  Real _time;
  Real _energy;
  Point _location;
};
