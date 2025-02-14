/*
   This file is part of FERRET, an add-on module for MOOSE

   FERRET is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   For help with FERRET please contact J. Mangeri <john.m.mangeri@gmail.com>
   and be sure to track new changes at github.com/mangerij/ferret

**/

#include "HeatFlowElectricT.h"
#include "Material.h"

registerMooseObject("FerretApp", HeatFlowElectricT);

InputParameters
HeatFlowElectricT::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Calculates a residual contribution due to modified ohm's law");
  params.addRequiredCoupledVar("potential_E_int", "electrical potential");
  params.addRequiredCoupledVar("T", "temperature");
  return params;
}

HeatFlowElectricT::HeatFlowElectricT(const InputParameters & parameters)
  : Kernel(parameters),
    _potential_E_int_var(coupled("potential_E_int")),
    _potential_E_int(coupledValue("potential_E_int")),
    _potential_E_int_grad(coupledGradient("potential_E_int")),
    _T_var(coupled("T")),
    _T(coupledValue("T")),
    _T_grad(coupledGradient("T")),
    _ecC(getMaterialProperty<Real>("ecC")),
    _sbC(getMaterialProperty<Real>("sbC")),
    _thC(getMaterialProperty<Real>("thC"))
{
}

Real
HeatFlowElectricT::computeQpResidual()
{
  return ((-_grad_test[_i][_qp](0)) * (-_thC[_qp] * _T_grad[_qp](0)) + (-_grad_test[_i][_qp](0)) * (-_ecC[_qp] * _sbC[_qp] * _sbC[_qp] * _T[_qp] * _T_grad[_qp](0)) + (-_grad_test[_i][_qp](0)) * (-_ecC[_qp] * _sbC[_qp] * _T[_qp] * _potential_E_int_grad[_qp](0)) + _test[_i][_qp] * (-_ecC[_qp] * _potential_E_int_grad[_qp](0) * _potential_E_int_grad[_qp](0)) + _test[_i][_qp] * (-_ecC[_qp] * _sbC[_qp] * _T_grad[_qp](0) * _potential_E_int_grad[_qp](0))) + ((-_grad_test[_i][_qp](1)) * (-_thC[_qp] * _T_grad[_qp](1)) + (-_grad_test[_i][_qp](1)) * (-_ecC[_qp] * _sbC[_qp] * _sbC[_qp] * _T[_qp] * _T_grad[_qp](1)) + (-_grad_test[_i][_qp](1)) * (-_ecC[_qp] * _sbC[_qp] * _T[_qp] * _potential_E_int_grad[_qp](1)) + _test[_i][_qp] * (-_ecC[_qp] * _potential_E_int_grad[_qp](1) * _potential_E_int_grad[_qp](1)) +
          _test[_i][_qp] * (-_ecC[_qp] * _sbC[_qp] * _T_grad[_qp](1) * _potential_E_int_grad[_qp](1))) + ((-_grad_test[_i][_qp](2)) * (-_thC[_qp] * _T_grad[_qp](2)) +
          (-_grad_test[_i][_qp](2)) * (-_ecC[_qp] * _sbC[_qp] * _sbC[_qp] * _T[_qp] * _T_grad[_qp](2)) + (-_grad_test[_i][_qp](2)) * (-_ecC[_qp] * _sbC[_qp] * _T[_qp] * _potential_E_int_grad[_qp](2)) +
          _test[_i][_qp] * (-_ecC[_qp] * _potential_E_int_grad[_qp](2) * _potential_E_int_grad[_qp](2)) + _test[_i][_qp] * (-_ecC[_qp] * _sbC[_qp] * _T_grad[_qp](2) * _potential_E_int_grad[_qp](2)));
}

Real
HeatFlowElectricT::computeQpJacobian()
{
  return ((-_grad_test[_i][_qp](0) *
               (-_ecC[_qp] * _sbC[_qp] * _phi[_j][_qp] * _potential_E_int_grad[_qp](0)) +
           (-_grad_test[_i][_qp](0)) * (-_thC[_qp] * _grad_phi[_j][_qp](0)) +
           (-_grad_test[_i][_qp](0)) *
               (-_sbC[_qp] * _sbC[_qp] * _ecC[_qp] *
                (_phi[_j][_qp] * _T_grad[_qp](0) + _T[_qp] * _grad_phi[_j][_qp](0))) +
           _test[_i][_qp] *
               (-_sbC[_qp] * _ecC[_qp] * _grad_phi[_j][_qp](0) * _potential_E_int_grad[_qp](0))) +
          ((-_grad_test[_i][_qp](1)) *
               (-_ecC[_qp] * _sbC[_qp] * _phi[_j][_qp] * _potential_E_int_grad[_qp](1)) +
           (-_grad_test[_i][_qp](1)) * (-_thC[_qp] * _grad_phi[_j][_qp](1)) +
           (-_grad_test[_i][_qp](1)) *
               (-_sbC[_qp] * _sbC[_qp] * _ecC[_qp] *
                (_phi[_j][_qp] * _T_grad[_qp](1) + _T[_qp] * _grad_phi[_j][_qp](1))) +
           _test[_i][_qp] *
               (-_sbC[_qp] * _ecC[_qp] * _grad_phi[_j][_qp](1) * _potential_E_int_grad[_qp](1))) +
          ((-_grad_test[_i][_qp](2)) *
               (-_ecC[_qp] * _sbC[_qp] * _phi[_j][_qp] * _potential_E_int_grad[_qp](2)) +
           (-_grad_test[_i][_qp](2)) * (-_thC[_qp] * _grad_phi[_j][_qp](2)) +
           (-_grad_test[_i][_qp](2)) *
               (-_sbC[_qp] * _sbC[_qp] * _ecC[_qp] *
                (_phi[_j][_qp] * _T_grad[_qp](2) + _T[_qp] * _grad_phi[_j][_qp](2))) +
           _test[_i][_qp] *
               (-_sbC[_qp] * _ecC[_qp] * _grad_phi[_j][_qp](2) * _potential_E_int_grad[_qp](2))));
}

Real
HeatFlowElectricT::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _potential_E_int_var)
  {
    return (
        (-_grad_test[_i][_qp](0)) * (-_ecC[_qp] * _sbC[_qp] * _T[_qp] * _grad_phi[_j][_qp](0)) -
        _test[_i][_qp] * (2.0 * _ecC[_qp] * _potential_E_int_grad[_qp](0) * _grad_phi[_j][_qp](0) +
                          _ecC[_qp] * _sbC[_qp] * _T_grad[_qp](0) * _grad_phi[_j][_qp](0)) +
        (-_grad_test[_i][_qp](1)) * (-_ecC[_qp] * _sbC[_qp] * _T[_qp] * _grad_phi[_j][_qp](1)) -
        _test[_i][_qp] * (2.0 * _ecC[_qp] * _potential_E_int_grad[_qp](1) * _grad_phi[_j][_qp](1) +
                          _ecC[_qp] * _sbC[_qp] * _T_grad[_qp](1) * _grad_phi[_j][_qp](1)) +
        (-_grad_test[_i][_qp](2)) * (-_ecC[_qp] * _sbC[_qp] * _T[_qp] * _grad_phi[_j][_qp](2)) -
        _test[_i][_qp] * (2.0 * _ecC[_qp] * _potential_E_int_grad[_qp](2) * _grad_phi[_j][_qp](2) +
                          _ecC[_qp] * _sbC[_qp] * _T_grad[_qp](2) * _grad_phi[_j][_qp](2)));
  }
  else
    return 0.0;
}
