#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "pybind11/pybind11.h" // must be first
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define FORCE_IMPORT_ARRAY
#include "modmesh/python/python.hpp"

#include "spacetime/python/wrapper_linear_scalar.hpp"
#include "spacetime/python/wrapper_inviscid_burgers.hpp"
#include "spacetime/python/wrapper_spacetime.hpp"
#include "spacetime/python/WrapBase.hpp"

namespace spacetime
{
namespace python
{
namespace detail
{

template< typename WST, typename WCET, typename WSET >
void add_solver(pybind11::module & mod, const std::string & name, const std::string & desc)
{
    WST::commit
    (
        mod
      , (name + "Solver").c_str()
      , ("Solving algorithm of " + desc).c_str()
    );
    WCET::commit
    (
        mod
      , (name + "Celm").c_str()
      , ("Conservation element of " + desc).c_str()
    );
    WSET::commit
    (
        mod
      , (name + "Selm").c_str()
      , ("Solution element of " + desc).c_str()
    );
}

inline void initialize_impl(pybind11::module & mod)
{
    namespace mpy = modmesh::python;
    namespace spy = spacetime::python;

    mod.doc() = "_libst: One-dimensional space-time CESE method code";

    pybind11::module::import("modmesh._modmesh");

    spy::WrapGrid::commit(mod, "Grid", "Spatial grid data");
    spy::WrapKernel::commit(mod, "Kernel", "Solution element calculation hooks");
    spy::WrapField::commit(mod, "Field", "Solution data");

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

} /* end namespace detail */

struct init_tag;

inline void initialize(pybind11::module & mod)
{
    modmesh::python::OneTimeInitializer<init_tag>::me()(mod, true, detail::initialize_impl);
}

} /* end namespace python */
} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
