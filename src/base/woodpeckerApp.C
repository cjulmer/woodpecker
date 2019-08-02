#include "woodpeckerApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

#include "ReactionRateAux.h"

template <>
InputParameters
validParams<woodpeckerApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

woodpeckerApp::woodpeckerApp(InputParameters parameters) : MooseApp(parameters)
{
  woodpeckerApp::registerAll(_factory, _action_factory, _syntax);
}

woodpeckerApp::~woodpeckerApp() {}

void
woodpeckerApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"woodpeckerApp"});
  Registry::registerActionsTo(af, {"woodpeckerApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
woodpeckerApp::registerApps()
{
  registerApp(woodpeckerApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
woodpeckerApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  woodpeckerApp::registerAll(f, af, s);
}
extern "C" void
woodpeckerApp__registerApps()
{
  woodpeckerApp::registerApps();
}
