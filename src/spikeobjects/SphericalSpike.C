#include "SphericalSpike.h"

SphericalSpike::SphericalSpike(Real time, Real energy, Real x, Real y, Real z)
  : _time(time), _energy(energy), _location(Point(x, y, z))
{
}

SphericalSpike::SphericalSpike(Real time, Real energy, Point location)
  : _time(time), _energy(energy), _location(location)
{
}

Real
SphericalSpike::getTime() const
{
  return _time;
}

Real
SphericalSpike::getEnergy() const
{
  return _energy;
}

Point
SphericalSpike::getLocation() const
{
  return _location;
}
