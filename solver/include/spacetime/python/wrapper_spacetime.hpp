#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "spacetime.hpp"
#include "spacetime/python/WrapBase.hpp"

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
        (*this)
            .init()
            .str()
            .def_property_readonly("xmin", &wrapped_type::xmin)
            .def_property_readonly("xmax", &wrapped_type::xmax)
            .def_property_readonly("nelement", &wrapped_type::nelement)
            .def("element", &wrapped_type::element_at)
        ;
    }

    wrapper_type & init()
    {
        return def(pybind11::init([](real_type xmin, real_type xmax, size_t nelm) {
            std::shared_ptr<Grid> grid = Grid::construct(xmin, xmax, nelm);
            return grid;
        }));
    }

    wrapper_type & str()
    {
        return def(
            "__str__",
            [](wrapped_type & self) {
                return Formatter()
                    << "Grid(" << "xmin=" << self.xmin() << ", xmax=" << self.xmax() << ", "
                    << "nelement=" << self.nelement() << ")"
                    >> Formatter::to_str;
            }
        );
    }

}; /* end class WrapGrid */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapElement
  : public WrapBase< WrapElement, Element >
{

    friend base_type;

    WrapElement(pybind11::module & mod, const char * pyname, const char * clsdoc)
        : base_type(mod, pyname, clsdoc)
    {
        (*this)
            .str()
            .def("duplicate", &wrapped_type::duplicate)
            .def_property_readonly("grid", &wrapped_type::grid)
            .def_property_readonly("index", &wrapped_type::index)
            .def_property_readonly("on_even_plane", &wrapped_type::on_even_plane)
            .def_property_readonly("x", &wrapped_type::x)
            .def_property_readonly("xneg", &wrapped_type::xneg)
            .def_property_readonly("xpos", &wrapped_type::xpos)
            .def("move", &wrapped_type::move_at)
            .def("move_left", &wrapped_type::move_left_at)
            .def("move_right", &wrapped_type::move_right_at)
            .def("move_neg", &wrapped_type::move_neg_at)
            .def("move_pos", &wrapped_type::move_pos_at)
        ;
    }

    wrapper_type & str()
    {
        return def(
            "__str__",
            [](wrapped_type & self) {
                return Formatter()
                    << "Element(" << (self.on_even_plane() ? "even" : "odd") << ", "
                    << "index=" << self.index() << ", x=" << self.x() << ", "
                    << "xneg=" << self.xneg() << ", xpos=" << self.xpos() << ")"
                    >> Formatter::to_str;
            }
        );
    }

}; /* end class WrapElement */

} /* end namespace python */

} /* end namespace spacetime */

// vim: set et sw=4 ts=4:
