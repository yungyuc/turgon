#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

/**
 * Inviscid Burgers' equation.
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/Field_decl.hpp"
#include "spacetime/SolutionBase_decl.hpp"
#include "spacetime/FelmBase.hpp"

namespace spacetime
{

class ConvectionFelm;

class ConvectionSolution
  : public SolutionBase<ConvectionSolution>
{

}; /* end class ConvectionSolution */

class ConvecitonFelm
  : public FelmBase<ConvectionFelm, ConvectionSolution>
{

}; /* end class ConvectionFelm */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
