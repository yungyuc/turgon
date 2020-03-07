/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/python.hpp" // must be first
#include "modmesh/python/python.hpp"

#include "spacetime.hpp"

#include <algorithm>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

namespace
{

template< typename WST, typename WCET, typename WSET >
void add_solver(pybind11::module & mod, const std::string & name, const std::string & desc)
{
    WST::commit
    (
        &mod
      , (name + "Solver").c_str()
      , ("Solving algorithm of " + desc).c_str()
    );
    WCET::commit
    (
        &mod
      , (name + "Celm").c_str()
      , ("Conservation element of " + desc).c_str()
    );
    WSET::commit
    (
        &mod
      , (name + "Selm").c_str()
      , ("Solution element of " + desc).c_str()
    );
}

} /* end namespace */

PYBIND11_MODULE(_libst, mod) // NOLINT
{
    namespace mpy = modmesh::python;
    namespace spy = spacetime::python;

    mod.doc() = "_libst: One-dimensional space-time CESE method code";

    mpy::initialize(mod); // numpy is imported here as early as possible or it crashes.

    spy::WrapGrid::commit(&mod, "Grid", "Spatial grid data");
    spy::WrapField::commit(&mod, "Field", "Solution data");

    add_solver
    <
        spy::WrapSolver
      , spy::WrapCelm
      , spy::WrapSelm
    >(mod, "", "no equation");

    add_solver
    <
        spy::WrapLinearScalarSolver
      , spy::WrapLinearScalarCelm
      , spy::WrapLinearScalarSelm
    >(mod, "LinearScalar", "a linear scalar equation");

    add_solver<
        spy::WrapInviscidBurgersSolver
      , spy::WrapInviscidBurgersCelm
      , spy::WrapInviscidBurgersSelm
    >(mod, "InviscidBurgers", "the inviscid Burgers equation");
}

// vim: set et sw=4 ts=4:
