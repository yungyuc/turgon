#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/Grid_decl.hpp"
#include "spacetime/Celm_decl.hpp"

namespace spacetime
{

inline
Grid::Grid(real_type xmin, real_type xmax, size_t ncelm, ctor_passkey const &)
  : m_xmin(xmin), m_xmax(xmax), m_ncelm(ncelm)
{
    if (ncelm < 1)
    {
        throw std::invalid_argument(Formatter()
            << "Grid::Grid(xmin=" << xmin << ", xmax=" << xmax
            << ", ncelm=" << ncelm << ") invalid argument: ncelm smaller than 1"
        );
    }
    if (xmin >= xmax)
    {
        throw std::invalid_argument(Formatter()
            << "Grid::Grid(xmin=" << xmin << ", xmax=" << xmax
            << ", ncelm=" << ncelm << ") invalid arguments: xmin >= xmax"
        );
    }
    // Fill the array for CCE boundary.
    const real_type xspace = (xmax - xmin) / ncelm;
    array_type xloc(std::vector<size_t>{ncelm+1});
    xloc[0] = xmin;
    for (size_t it=1; it<ncelm; ++it)
    {
        xloc[it] = xloc[it-1] + xspace;
    }
    xloc[ncelm] = xmax;
    // Initialize.
    init_from_array(xloc);
}

inline
void Grid::init_from_array(array_type const & xloc)
{
    if (xloc.size() < 2)
    {
        throw std::invalid_argument(Formatter()
            << "Grid::init_from_array(xloc) invalid arguments: "
            << "xloc.size()=" << xloc.size() << " smaller than 2"
        );
    }
    for (size_t it=0; it<xloc.size()-1; ++it)
    {
        if (xloc[it] >= xloc[it+1])
        {
            throw std::invalid_argument(Formatter()
                << "Grid::init_from_array(xloc) invalid arguments: "
                << "xloc[" << it << "]=" << xloc[it]
                << " >= xloc[" << it+1 << "]=" << xloc[it+1]
            );
        }
    }
    m_ncelm = xloc.size() - 1;
    m_xmin = xloc[0];
    m_xmax = xloc[m_ncelm];
    // Mark the boundary of conservation celms.
    const size_t nx = m_ncelm*2+3;
    m_xcoord = array_type(std::vector<size_t>{nx});
    // Fill x-coordinates at CE boundary.
    for (size_t it=0; it<xloc.size(); ++it)
    {
        m_xcoord[it*2+1] = xloc[it];
    }
    // Fill x-coordinates at CE center.
    for (size_t it=0; it<m_ncelm; ++it)
    {
        m_xcoord[it*2+2] = (m_xcoord[it*2+1] + m_xcoord[it*2+3])/2;
    }
    // Fill the front and back values.
    m_xcoord[0] = m_xcoord[1] + m_xcoord[1] - m_xcoord[2];
    m_xcoord[nx-1] = m_xcoord[nx-2] + m_xcoord[nx-2] - m_xcoord[nx-3];
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
