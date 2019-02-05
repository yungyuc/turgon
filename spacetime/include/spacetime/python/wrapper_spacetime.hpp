#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/python/common.hpp"

namespace spacetime
{

namespace python
{

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapGrid
  : public WrapBase< WrapGrid, Grid, std::shared_ptr<Grid> >
{

    friend base_type;

    WrapGrid(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def(
                py::init([](real_type xmin, real_type xmax, size_t nelm) {
                    return Grid::construct(xmin, xmax, nelm);
                }),
                py::arg("xmin"), py::arg("xmax"), py::arg("nelm")
            )
            .def(
                py::init([](xt::pyarray<wrapped_type::value_type> & xloc) {
                    return Grid::construct(xloc);
                }),
                py::arg("xloc")
            )
            .def("__str__", &detail::to_str<wrapped_type>)
            .def_property_readonly("xmin", &wrapped_type::xmin)
            .def_property_readonly("xmax", &wrapped_type::xmax)
            .def_property_readonly("ncelm", &wrapped_type::ncelm)
            .def_property_readonly("nselm", &wrapped_type::nselm)
            .def_property_readonly(
                "xcoord",
                static_cast<wrapped_type::array_type & (wrapped_type::*)()>(&wrapped_type::xcoord)
            )
            .def_property_readonly_static("BOUND_COUNT", [](py::object){ return Grid::BOUND_COUNT; })
        ;
    }

}; /* end class WrapGrid */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapField
  : public WrapBase< WrapField, Field, std::shared_ptr<Field> >
{

    friend base_type;

    WrapField(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def("__str__", &detail::to_str<wrapped_type>)
            .def_property_readonly("grid", [](wrapped_type & self){ return self.grid().shared_from_this(); })
            .def_property_readonly("nvar", &wrapped_type::nvar)
            .def_property(
                "time_increment",
                &wrapped_type::time_increment,
                &wrapped_type::set_time_increment
             )
            .def_property_readonly("dt", &wrapped_type::dt)
            .def_property_readonly("hdt", &wrapped_type::hdt)
            .def_property_readonly("qdt", &wrapped_type::qdt)
            .def(
                "celm",
                //static_cast<Celm (wrapped_type::*)(ssize_t, bool)>(&wrapped_type::celm_at),
                &wrapped_type::celm_at<Celm>,
                py::arg("ielm"), py::arg("odd_plane")=false
            )
            .def(
                "selm",
                //static_cast<Selm (wrapped_type::*)(ssize_t, bool)>(&wrapped_type::selm_at),
                &wrapped_type::selm_at<Selm>,
                py::arg("ielm"), py::arg("odd_plane")=false
            )
        ;
    }

}; /* end class WrapField */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapSolver
  : public WrapSolverBase< WrapSolver, Solver >
{

    using base_type = WrapSolverBase< WrapSolver, Solver >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;
    friend base_type::base_type;

    WrapSolver(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def(
                py::init([](std::shared_ptr<Grid> const & grid, size_t nvar, typename wrapped_type::value_type time_increment) {
                    return wrapped_type::construct(grid, nvar, time_increment);
                }),
                py::arg("grid"), py::arg("nvar"), py::arg("time_increment")
            )
        ;
    }

}; /* end class WrapSolver */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapCelm
  : public WrapElementBase< WrapCelm, Celm >
{

    using base_type = WrapElementBase< WrapCelm, Celm >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;
    friend base_type::base_type;

    WrapCelm(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        (*this)
            .def_property_readonly("selm_xn", &wrapped_type::selm_xn)
            .def_property_readonly("selm_xp", &wrapped_type::selm_xp)
            .def_property_readonly("selm_tn", &wrapped_type::selm_tp)
            .def_property_readonly("selm_tp", &wrapped_type::selm_tp)
        ;
    }

}; /* end class WrapCelm */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapSelm
  : public WrapElementBase< WrapSelm, Selm >
{

    using base_type = WrapElementBase< WrapSelm, Selm >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;
    friend base_type::base_type;

    WrapSelm(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        (*this)
            .def_property_readonly("dxneg", &wrapped_type::dxneg)
            .def_property_readonly("dxpos", &wrapped_type::dxpos)
            .def(
                "so0",
                static_cast<wrapped_type::value_type const & (wrapped_type::*)(size_t) const>(&wrapped_type::so0)
            )
            .def(
                "set_so0",
                [](wrapped_type & self, size_t it, wrapped_type::value_type val) {
                    self.so0(it) = val;
                }
            )
            .def(
                "so1",
                static_cast<wrapped_type::value_type const & (wrapped_type::*)(size_t) const>(&wrapped_type::so1)
            )
            .def(
                "set_so1",
                [](wrapped_type & self, size_t it, wrapped_type::value_type val) {
                    self.so1(it) = val;
                }
            )
        ;
    }

}; /* end class WrapSelm */

} /* end namespace python */

} /* end namespace spacetime */

// vim: set et sw=4 ts=4:
