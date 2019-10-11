#include "RandomTest.h"

#include "MooseRandom.h"
#include "RandomTestSub.h"

registerMooseObject("woodpeckerApp", RandomTest);

template <>
InputParameters
validParams<RandomTest>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_BEGIN;
  return params;
}

RandomTest::RandomTest(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _random(declareRestartableData<MooseRandom>("random_generator"))
{
  // Seed the random number generator
  _random.seed(0, 3);

  _debug.open("Random1.txt", std::ofstream::out);
  _debug.close();
  _debug.open("Random2.txt", std::ofstream::out);
  _debug.close();
}

void
RandomTest::execute()
{
  //_debug.open("Event.txt", std::ofstream::out | std::ofstream::app);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    _random.saveState();
    _debug.open("Random1.txt", std::ofstream::out | std::ofstream::app);
    for (auto i = 0; i < 100; ++i)
    {
      _debug << _random.rand(0) << "\n";
    }
    _debug.close();

    _random.restoreState();
    //_debug.open("Random2.txt", std::ofstream::out | std::ofstream::app);
    for (auto i = 0; i < 25; ++i)
    {
      _debug.open("Random2.txt", std::ofstream::out | std::ofstream::app);
      _debug << _random.rand(0) << "\n";
      _debug.close();
      RandomTestSub randomtestsub(_random);
    }
    //_debug.close();
  }

}
