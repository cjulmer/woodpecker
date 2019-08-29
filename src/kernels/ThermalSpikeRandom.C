#include "ThermalSpikeRandom.h"

registerMooseObject("woodpeckerApp", ThermalSpikeRandom);

template <>
InputParameters
validParams<ThermalSpikeRandom>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<Real>("specific_heat", "Specific heat of the material");
  params.addRequiredParam<Real>("density_name", "Density of the material");
  params.addRequiredParam<Real>("delta_T_max", "Maximum temperature increase due to thermal spike");
  params.addRequiredParam<Real>("energy", "Energy of thermal spike");
  params.addRequiredParam<Real>("spike_rate", "Rate of thermal spikes");
  params.addRequiredParam<Real>("time", "Time of thermal spike initiation");
  return params;
}

ThermalSpikeRandom::ThermalSpikeRandom(const InputParameters & parameters)
  : Kernel(parameters),
    _specific_heat(getParam<Real>("specific_heat")),
    _density(getParam<Real>("density_name")),
    _delta_T_max(getParam<Real>("delta_T_max")),
    _energy(getParam<Real>("energy")),
    _spike_rate(getParam<Real>("spike_rate")),
    _initiation_time(getParam<Real>("time"))
{
  setRandomResetFrequency(EXEC_TIMESTEP_BEGIN);
  fout.open("test.txt");
  fout.close();
  _dim = 2;

  _point.resize(2,10.0);
  _t_now = -1.0;
  _dt_now = -1.0;

  /* CANNOT USE GetRandomReal() in constructor!
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
  {
    _spikes.push_back(getSpikeInterval()+_initiation_time);
  }
  */

  _b = std::sqrt(_energy / (2 * 3.14159265359 * _density * _specific_heat * _delta_T_max));
}

Real
ThermalSpikeRandom::computeQpResidual()
{

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (_t != _t_now || _dt != _dt_now)
  {
    fout.open("test.txt", std::ofstream::out | std::ofstream::app);
    //MPI_Barrier(MPI_COMM_WORLD);
    if (size > 1)
    {
      int data_size;
      if (rank == 0)
      {
        if (_spikes.empty())
        {
          _spikes.push_back(getSpikeInterval()+_initiation_time);
        }
        while (_spikes.size() > 1)
        {
          _spikes.pop_front();
        }
        while (_spikes.back() <= _t)
        {
          _spikes.push_back(_spikes.back()+getSpikeInterval());
        }

        _points.resize(0);
        for (auto i = 0; i < (_spikes.size() - 1) * _dim; ++i)
        {
          _points.push_back(getRandomReal()*20.0);
        }
        data_size = _points.size();
        MPI_Bcast(&data_size,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(_points.data(), _points.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      }
      else
      {
        MPI_Bcast(&data_size,1,MPI_INT,0,MPI_COMM_WORLD);
        _points.resize(data_size);
        MPI_Bcast(_points.data(), _points.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      }
    }
    else
    {
      //setRandomData();
    }
    _t_now = _t;
    _dt_now = _dt;
    fout << "Process " << rank << " after" << "\n";
    fout << "  t " << _t << " : " << _t_now << "\n";
    fout << "  dt " << _dt << " : " << _dt_now << "\n";
    if (rank == 0)
    {
      for (auto i = 0; i < (_spikes.size()-1); ++i)
      {
        fout << "    " << _spikes[i];
        for (auto j = 0; j < _dim; ++j)
        {
          fout << " " << _points[i*_dim+j];
        }
        fout << "\n";
      }
      for (auto i = 0; i < _spikes.size(); ++i)
      {
        fout << " " << _spikes[i];
      }
      fout << "\n";
    }
    for (auto i = 0; i < _points.size(); ++i)
    {
      fout << " " << _points[i];
    }
    fout << "\n";
    fout.close();
  }

  Real factor = 0;
  Real x = _q_point[_qp](0);
  Real y = _q_point[_qp](1);
  for (auto i = 0; i < _points.size()/_dim; ++i)
  {
    // point[0] = _points[i*_dim]
    // point[1] = _points[i*_dim+1]
    factor += std::exp(-((_points[i*_dim] - x) * (_points[i*_dim] - x)
        + (_points[i*_dim+1] - y) * (_points[i*_dim+1] - y)) / (2 * _b * _b));
  }
  return _test[_i][_qp] * -factor / _dt * _specific_heat * _density * _delta_T_max;

  // if (_time >= (_t - _dt) && _time < _t)
  /*Point p = _q_point[_qp];
  Real x = _q_point[_qp](0);
  Real y = _q_point[_qp](1);
  Real factor = _specific_heat * _density * _delta_T_max *
                std::exp(-((_point[0] - x) * (_point[0] - x) + (_point[1] - y) * (_point[1] - y)) /
                         (2 * _b * _b));
  return _test[_i][_qp] * -factor / _dt;*/
}

void
ThermalSpikeRandom::setRandomData()
{
  _point.resize(0);
  for (auto i = 1; i < 3; ++i)
  {
    Real val = getRandomReal();
    val *= 20.0;
    _point.push_back(val);
  }
  return;
}

Real
ThermalSpikeRandom::getSpikeInterval()
{
  return - std::log(1.0 - getRandomReal()) / _spike_rate;
}
