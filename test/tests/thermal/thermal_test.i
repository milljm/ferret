[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 3
  ny = 3
  nz = 23
  xmin = -1
  xmax = 1
  ymin = -1
  ymax = 1
  zmin = -8
  zmax = 8
  elem_type = HEX8
[]

[GlobalParams]
  len_scale = 1.0
  alpha0 = 0.0003766

  Tc = 765.1

  alpha11 = -0.07253
  alpha111 = 0.26
  alpha12 = 0.75
  alpha112 = 0.61
  alpha123 = -3.67

  polar_x = polar_x
  polar_y = polar_y
  polar_z = polar_z
  potential_E_int = potential_E_int

  temperature = temperature
[]

[Materials]
  [./Landau_G]
    type = GenericConstantMaterial
    prop_names = 'G110 G11_G110 G12_G110 G44_G110 G44P_G110'
    prop_values = '0.15 0.6 0.0 0.3 0.3'
  [../]
[]

[Variables]
  [./polar_x]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = RandomIC
      min = -0.1e-2
      max = 0.1e-2
      seed = 5
      legacy_generator = true
    [../]
  [../]
  [./polar_y]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = RandomIC
      min = -0.1e-2
      max = 0.1e-2
      seed = 5
      legacy_generator = true
    [../]
  [../]
  [./polar_z]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = RandomIC
      min = -0.1e-2
      max = 0.1e-2
      seed = 5
      legacy_generator = true
    [../]
  [../]
  [./potential_E_int]
    order = FIRST
    family = LAGRANGE
  [../]
  [./temperature]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  #Bulk energy density
  [./bed_x]
    type = BulkEnergyDerivativeSixthCoupledT
    variable = polar_x
    component = 0
  [../]
  [./bed_y]
    type = BulkEnergyDerivativeSixthCoupledT
    variable = polar_y
    component = 1
  [../]
  [./bed_z]
    type = BulkEnergyDerivativeSixthCoupledT
    variable = polar_z
    component = 2
  [../]

  ##Wall energy penalty
  [./walled_x]
     type=WallEnergyDerivative
     variable = polar_x
     component = 0
  [../]
  [./walled_y]
     type=WallEnergyDerivative
     variable = polar_y
     component = 1
  [../]
  [./walled_z]
     type=WallEnergyDerivative
     variable = polar_z
     component = 2
  [../]

  ##Electrostatics
  [./polar_x_electric_E]
     type=PolarElectricEStrong
     variable = potential_E_int
     block = '0'
  [../]
  [./FE_E_int]
     type=Electrostatics
     variable = potential_E_int
     block = '0'
  [../]
  [./polar_electric_px]
     type=PolarElectricPStrong
     variable = polar_x
     component = 0
  [../]
  [./polar_electric_py]
     type=PolarElectricPStrong
     variable = polar_y
     component = 1
  [../]
  [./polar_electric_pz]
     type=PolarElectricPStrong
     variable = polar_z
     component = 2
  [../]

  #temperature kernel
  [./t_dif]
     type = Diffusion
     variable = temperature
  [../]



  ##Time dependence
  [./polar_x_time]
     type = TimeDerivativeScaled
     variable=polar_x
    time_scale = 1.0
  [../]
  [./polar_y_time]
     type = TimeDerivativeScaled
     variable=polar_y
    time_scale = 1.0
  [../]
  [./polar_z_time]
     type = TimeDerivativeScaled
     variable = polar_z
    time_scale = 1.0
  [../]
[]

[BCs]
  [./potential_cube5]
    type = DirichletBC
    boundary = 'front'
    value = 0.002
    variable = potential_E_int
  [../]
  [./potential_cube6]
    type = DirichletBC
    boundary = 'back'
    value = 0.002
    variable = potential_E_int
  [../]

  [./t5]
    type = DirichletBC
    boundary = 'front'
    value = 0.0
    variable = temperature
  [../]
  [./t6]
    type = DirichletBC
    boundary = 'back'
    value = 825
    variable = temperature
  [../]

[]

[Materials]
  [./permitivitty_1]
    type = GenericConstantMaterial
    prop_names = 'permittivity'
    prop_values = '0.5843763'
  [../]
[]


[Preconditioning]
  [./smp]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_gmres_restart -snes_atol -snes_rtol -ksp_rtol -pc_type    -pc_factor_zeropivot'
    petsc_options_value = '    121             1e-10       1e-8       1e-6   bjacobi           1e-50     '
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'       #"PJFNK, JFNK, NEWTON"
  scheme = 'implicit-euler'   #"implicit-euler, explicit-euler, crank-nicolson, bdf2, rk-2"
  dtmin = 1e-13
  dtmax = 0.5
  num_steps = 10
[]

[Outputs]
  print_linear_residuals = false
  [./out]
    type = Exodus
    file_base = out_thermal_test
    elemental_as_nodal = true
  [../]
[]
