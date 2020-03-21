#pragma once

/*
 * Copyright (c) 2017, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <memory>
#include <type_traits>

PYBIND11_DECLARE_HOLDER_TYPE(T, std::unique_ptr<T>);
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

#ifdef __GNUG__
#  define SPACETIME_PYTHON_WRAPPER_VISIBILITY __attribute__((visibility("hidden")))
#else
#  define SPACETIME_PYTHON_WRAPPER_VISIBILITY
#endif

namespace spacetime
{

namespace python
{

template
<
    class Wrapper
  , class Wrapped
  , class Holder = std::unique_ptr<Wrapped>
  , class WrappedBase = Wrapped
>
using WrapBase = modmesh::python::WrapBase<Wrapper, Wrapped, Holder, WrappedBase>;

} /* end namespace python */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
