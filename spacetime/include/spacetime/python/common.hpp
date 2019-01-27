#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <list>
#include <functional>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "xtensor-python/pyarray.hpp"

#include <sstream>

#include "spacetime.hpp"
#include "spacetime/python/WrapBase.hpp"

namespace spacetime
{

namespace python
{

namespace detail {

template<class T> std::string to_str(T const & self) { return Formatter() << self >> Formatter::to_str; }

} /* end namespace detail */

template<class WT, class ET>
class
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
            .def("__str__", &detail::to_str<wrapped_type>)
            .def("duplicate", &wrapped_type::duplicate)
            .def_property_readonly("dup", &wrapped_type::duplicate)
            .def_property_readonly("x", &wrapped_type::x)
            .def_property_readonly("dx", &wrapped_type::dx)
            .def_property_readonly("xneg", &wrapped_type::xneg)
            .def_property_readonly("xpos", &wrapped_type::xpos)
            .def_property_readonly("xctr", &wrapped_type::xctr)
            .def_property_readonly("index", &wrapped_type::index)
            .def_property_readonly("on_even_plane", &wrapped_type::on_even_plane)
            .def_property_readonly("on_odd_plane", &wrapped_type::on_odd_plane)
            .def_property_readonly("grid", &wrapped_type::grid)
            .def_property_readonly(
                "field"
              , static_cast<Field & (wrapped_type::*)()>(&wrapped_type::field)
            )
            .def_property_readonly("time_increment", &wrapped_type::time_increment)
            .def_property_readonly("dt", &wrapped_type::dt)
            .def_property_readonly("hdt", &wrapped_type::hdt)
            .def_property_readonly("qdt", &wrapped_type::qdt)
            .def("move", &wrapped_type::move_at)
            .def("move_left", &wrapped_type::move_left_at)
            .def("move_right", &wrapped_type::move_right_at)
            .def("move_neg", &wrapped_type::move_neg_at)
            .def("move_pos", &wrapped_type::move_pos_at)
        ;
    }

}; /* end class WrapElementBase */

template<class WT, class ST>
class
WrapSolutionBase
  : public WrapBase< WT, ST, std::shared_ptr<ST> >
{

public:

    using base_type = WrapBase< WT, ST, std::shared_ptr<ST> >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;

protected:

    WrapSolutionBase(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def("__str__", &detail::to_str<wrapped_type>)
            .def_property_readonly("grid", [](wrapped_type & self){ return self.grid().shared_from_this(); })
            .def_property_readonly("nvar", &wrapped_type::nvar)
            .def_property(
                "time_increment"
              , &wrapped_type::time_increment
              , &wrapped_type::set_time_increment
             )
            .def_property_readonly("dt", &wrapped_type::dt)
            .def_property_readonly("hdt", &wrapped_type::hdt)
            .def_property_readonly("qdt", &wrapped_type::qdt)
            .def(
                "celm"
              , static_cast<typename wrapped_type::celm_type (wrapped_type::*)(size_t, bool)>(&wrapped_type::celm_at)
              , py::arg("ielm"), py::arg("odd_plane")=false
            )
            .def(
                "selm"
              , static_cast<typename wrapped_type::selm_type (wrapped_type::*)(size_t, bool)>(&wrapped_type::selm_at)
              , py::arg("ielm"), py::arg("odd_plane")=false
            )
            .def_property_readonly(
                "so0"
              , static_cast<typename wrapped_type::array_type & (wrapped_type::*)()>(&wrapped_type::so0)
            )
            .def_property_readonly(
                "so1"
              , static_cast<typename wrapped_type::array_type & (wrapped_type::*)()>(&wrapped_type::so1)
            )
            .def("march_half_so0", &wrapped_type::march_half_so0, py::arg("odd_plane"))
            .def("march_half_so1", &wrapped_type::march_half_so1, py::arg("odd_plane"))
        ;
    }

}; /* end class WrapSolutionBase */

class ModuleInitializer {

public:

    using init_type = std::function<PyObject*(pybind11::module&)>;

    static ModuleInitializer & get_instance()
    {
        static ModuleInitializer inst;
        return inst;
    }

    void initialize(pybind11::module & topmod)
    {
        if (!m_initialized)
        {
            for (init_type initializer: m_initializers)
            {
                initializer(topmod);
            }
        }
        m_initialized = true;
    }

    bool is_initialized() const { return m_initialized; }

    ModuleInitializer & add(init_type init)
    {
        m_initializers.push_back(init);
        return *this;
    }

private:

    ModuleInitializer() = default;
    ModuleInitializer(ModuleInitializer const & ) = delete;
    ModuleInitializer(ModuleInitializer       &&) = delete;
    ModuleInitializer & operator=(ModuleInitializer const & ) = delete;
    ModuleInitializer & operator=(ModuleInitializer       &&) = delete;

    bool m_initialized = false;
    std::list<init_type> m_initializers;

}; /* end class ModuleInitializer */

} /* end namespace python */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
