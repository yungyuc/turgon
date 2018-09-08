/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <pybind11/pybind11.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <utility>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstring>

#include "spacetime.hpp"
#include "spacetime/python.hpp"

namespace spacetime
{

namespace python
{

PyObject * ModuleInitializer::initialize_spacetime(pybind11::module & mod)
{
    import_array1(nullptr); // or numpy c api segfault.

    mod.doc() = "_libst: One-dimensional space-time CESE method code";

    WrapGrid::commit(mod, "Grid", "Spatial grid");
    WrapCelm::commit(mod, "Celm", "Conservation element");
    WrapSelm::commit(mod, "Selm", "Solution element");
    WrapSolution::commit(mod, "Solution", "Solution field");

    return mod.ptr();
}

} /* end namespace python */

} /* end namespace march */

PYBIND11_MODULE(_libst, mod) {
    ::spacetime::python::ModuleInitializer::get_instance().initialize(mod);
}

// vim: set et sw=4 ts=4:
