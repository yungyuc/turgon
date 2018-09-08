#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <iostream>

#include "spacetime/ElementBase.hpp"
#include "spacetime/Grid.hpp"
#include "spacetime/Solution.hpp"

namespace spacetime
{

std::ostream& operator<<(std::ostream& os, const Grid & grid)
{
    os << "Grid(xmin=" << grid.xmin() << ", xmax=" << grid.xmax() << ", ncelm=" << grid.ncelm() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Solution & sol)
{
    os << "Solution(grid=" << sol.grid() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Celm & elm)
{
    os << "Celm(" << (elm.on_even_plane() ? "even" : "odd") << ", ";
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Selm & elm)
{
    os << "Selm(" << (elm.on_even_plane() ? "even" : "odd") << ", ";
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
