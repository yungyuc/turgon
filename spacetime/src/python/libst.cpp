/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/python.hpp" // must be first

#include <utility>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstring>

#include "spacetime.hpp"

namespace
{

PyObject * initialize_spacetime(pybind11::module & mod)
{
    using namespace spacetime::python;

    xt::import_numpy(); // or numpy c api segfault.

    mod.doc() = "_libst: One-dimensional space-time CESE method code";

    WrapGrid::commit(mod, "Grid", "Spatial grid");
    WrapSolver::commit(mod, "Solver", "Solving algorithm operating on the field");
    WrapCelm::commit(mod, "Celm", "Conservation element");
    WrapSelm::commit(mod, "Selm", "Solution element");

    return mod.ptr();
}

PyObject * initialize_linear_scalar(pybind11::module & mod)
{
    using namespace spacetime::python;

    xt::import_numpy(); // or numpy c api segfault.

    WrapLinearScalarSolver::commit
    (
        mod
      , "LinearScalarSolver"
      , "Solving algorithm for a linear scalar equation"
    );
    WrapLinearScalarSelm::commit
    (
        mod
      , "LinearScalarSelm"
      , "Solution element of a linear scalar equation"
    );

    return mod.ptr();
}

PyObject * initialize_inviscid_burgers(pybind11::module & mod)
{
    using namespace spacetime::python;

    xt::import_numpy(); // or numpy c api segfault.

    WrapInviscidBurgersSolver::commit
    (
        mod
      , "InviscidBurgersSolver"
      , "Solving algorithm of the inviscid Burgers equation"
    );
    WrapInviscidBurgersSelm::commit
    (
        mod
      , "InviscidBurgersFelm"
      , "Flux calculator for the solution element of the inviscid Burgers equation"
    );

    return mod.ptr();
}

} /* end namespace */

PYBIND11_MODULE(_libst, mod) {
    ::spacetime::python::ModuleInitializer::get_instance()
        .add(initialize_spacetime)
        .add(initialize_linear_scalar)
        .add(initialize_inviscid_burgers)
        .initialize(mod)
    ;
}

// vim: set et sw=4 ts=4:
