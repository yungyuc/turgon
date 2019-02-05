#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/SolverBase_decl.hpp"

namespace spacetime
{

template< typename ST, typename CE, typename SE > inline
void SolverBase<ST,CE,SE>::march_half_so0(bool odd_plane)
{
    const sindex_type start = odd_plane ? -1 : 0;
    const sindex_type stop = grid().ncelm();
    for (sindex_type ic=start; ic<stop; ++ic)
    {
        auto ce = celm(ic, odd_plane);
        ce.selm_tp().so0(0) = ce.calc_so0(0);
    }
}

template< typename ST, typename CE, typename SE > inline
void SolverBase<ST,CE,SE>::march_half_so1(bool odd_plane)
{
    const sindex_type start = odd_plane ? -1 : 0;
    const sindex_type stop = grid().ncelm();
    for (sindex_type ic=start; ic<stop; ++ic)
    {
        auto ce = celm(ic, odd_plane);
        ce.selm_tp().so1(0) = ce.calc_so1(0);
    }
}
} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
