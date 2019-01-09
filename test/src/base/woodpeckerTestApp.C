//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "woodpeckerTestApp.h"
#include "woodpeckerApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

template <>
InputParameters
validParams<woodpeckerTestApp>()
{
  InputParameters params = validParams<woodpeckerApp>();
  return params;
}

woodpeckerTestApp::woodpeckerTestApp(InputParameters parameters) : MooseApp(parameters)
{
  woodpeckerTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

woodpeckerTestApp::~woodpeckerTestApp() {}

void
woodpeckerTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  woodpeckerApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"woodpeckerTestApp"});
    Registry::registerActionsTo(af, {"woodpeckerTestApp"});
  }
}

void
woodpeckerTestApp::registerApps()
{
  registerApp(woodpeckerApp);
  registerApp(woodpeckerTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
woodpeckerTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  woodpeckerTestApp::registerAll(f, af, s);
}
extern "C" void
woodpeckerTestApp__registerApps()
{
  woodpeckerTestApp::registerApps();
}
