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
            .def_property_readonly("ncelm", &wrapped_type::ncelm)
            .def(
                "celm",
                static_cast<Celm (wrapped_type::*)(size_t, bool)>(&wrapped_type::celm_at),
                pybind11::arg("ielm"), pybind11::arg("odd_plane")=false
            )
            .def(
                "selm",
                static_cast<Selm (wrapped_type::*)(size_t, bool)>(&wrapped_type::selm_at),
                pybind11::arg("ielm"), pybind11::arg("odd_plane")=false
            )
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
                    << "ncelm=" << self.ncelm() << ")"
                    >> Formatter::to_str;
            }
        );
    }

}; /* end class WrapGrid */

template<class WT, class ET>
class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapElementBase
  : public WrapBase< WT, ET >
{

public:

    using base_type = WrapBase< WT, ET >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;

protected:

    using base_type::base_type;

    WrapElementBase(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        (*this)
            .def("duplicate", &wrapped_type::duplicate)
            .def_property_readonly("dup", &wrapped_type::duplicate)
            .def_property_readonly("grid", &wrapped_type::grid)
            .def_property_readonly("x", &wrapped_type::x)
            .def_property_readonly("xneg", &wrapped_type::xneg)
            .def_property_readonly("xpos", &wrapped_type::xpos)
        ;
    }

}; /* end class WrapElementBase */

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
            .str()
            .def_property_readonly("index", &wrapped_type::index)
            .def_property_readonly("on_even_plane", &wrapped_type::on_even_plane)
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
                    << "Celm(" << (self.on_even_plane() ? "even" : "odd") << ", "
                    << "index=" << self.index() << ", x=" << self.x() << ", "
                    << "xneg=" << self.xneg() << ", xpos=" << self.xpos() << ")"
                    >> Formatter::to_str;
            }
        );
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
            .str()
            .def_property_readonly("index", &wrapped_type::index)
            .def_property_readonly("on_even_plane", &wrapped_type::on_even_plane)
        ;
    }

    wrapper_type & str()
    {
        return def(
            "__str__",
            [](wrapped_type & self) {
                return Formatter()
                    << "Selm("
                    << "index=" << self.index() << ", x=" << self.x() << ")"
                    >> Formatter::to_str;
            }
        );
    }

}; /* end class WrapSelm */

} /* end namespace python */

} /* end namespace spacetime */

// vim: set et sw=4 ts=4:
