//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef WOODPECKERTESTAPP_H
#define WOODPECKERTESTAPP_H

#include "MooseApp.h"

class woodpeckerTestApp;

template <>
InputParameters validParams<woodpeckerTestApp>();

class woodpeckerTestApp : public MooseApp
{
public:
  woodpeckerTestApp(InputParameters parameters);
  virtual ~woodpeckerTestApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs = false);
};

#endif /* WOODPECKERTESTAPP_H */
