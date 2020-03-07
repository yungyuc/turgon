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

PYBIND11_MODULE(_libst, mod) // NOLINT
{
    spacetime::python::initialize(mod);
}

// vim: set et sw=4 ts=4:
