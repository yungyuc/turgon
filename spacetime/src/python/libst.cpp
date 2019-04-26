/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/python.hpp" // must be first

#include "spacetime.hpp"

#include <algorithm>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

namespace
{

PyObject * initialize_spacetime(pybind11::module * mod)
{
    using namespace spacetime::python; // NOLINT(google-build-using-namespace)
    xt::import_numpy(); // or numpy c api segfault.
    mod->doc() = "_libst: One-dimensional space-time CESE method code";
    WrapGrid::commit(mod, "Grid", "Spatial grid");
    return mod->ptr();
}

} /* end namespace */

PYBIND11_MODULE(_libst, mod) // NOLINT
{
    using namespace spacetime::python; // NOLINT(google-build-using-namespace)
    ModuleInitializer::get_instance()
        .add(initialize_spacetime)
        .add_solver<WrapSolver, WrapCelm, WrapSelm>
        (&mod, "", "no equation")
        .add_solver<WrapLinearScalarSolver, WrapLinearScalarCelm, WrapLinearScalarSelm>
        (&mod, "LinearScalar", "a linear scalar equation")
        .add_solver<WrapInviscidBurgersSolver, WrapInviscidBurgersCelm, WrapInviscidBurgersSelm>
        (&mod, "InviscidBurgers", "the inviscid Burgers equation")
        .initialize(&mod)
    ;
}

// vim: set et sw=4 ts=4:
