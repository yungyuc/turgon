#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/Solution_decl.hpp"
#include "spacetime/Selm_decl.hpp"

namespace spacetime
{

inline Selm Solution::selm(size_t ielm)
{
    return Selm(*this, ielm);
}

inline Selm Solution::selm(size_t ielm, bool odd_plane)
{
    return Selm(*this, ielm, odd_plane);
}

inline Selm Solution::selm_at(size_t ielm)
{
    const Selm elm = selm(ielm);
    if (elm.xindex() < 1 || elm.xindex() >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Grid::selm_at(ielm=" << ielm << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    return elm;
}

inline Selm Solution::selm_at(size_t ielm, bool odd_plane)
{
    const Selm elm = selm(ielm, odd_plane);
    if (elm.xindex() < 1 || elm.xindex() >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Grid::selm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    return elm;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
