#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/SolutionBase_decl.hpp"

namespace spacetime
{

template< typename ST, typename CE, typename SE > inline
void SolutionBase<ST,CE,SE>::march_half_so0(bool odd_plane)
{
    size_t ncelm = grid().ncelm();
    if (odd_plane) { ncelm -= 1;}
    for (size_t ic=0; ic<ncelm; ++ic)
    {
        auto ce = celm(ic, odd_plane);
        ce.selm_tp().so0(0) = ce.calc_so0(0);
    }
}

template< typename ST, typename CE, typename SE > inline
void SolutionBase<ST,CE,SE>::march_half_so1(bool odd_plane)
{
    size_t ncelm = grid().ncelm();
    if (odd_plane) { ncelm -= 1;}
    for (size_t ic=0; ic<ncelm; ++ic)
    {
        auto ce = celm(ic, odd_plane);
        ce.selm_tp().so1(0) = ce.calc_so1(0);
    }
}
} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
