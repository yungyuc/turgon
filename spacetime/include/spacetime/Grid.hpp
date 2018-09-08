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
    if (ncelm < 1) {
        throw std::invalid_argument(Formatter()
            << "Grid::Grid(xmin=" << xmin << ", xmax=" << xmax
            << ", ncelm=" << ncelm << ") invalid argument: ncelm smaller than 1"
        );
    }
    if (xmin >= xmax) {
        throw std::invalid_argument(Formatter()
            << "Grid::Grid(xmin=" << xmin << ", xmax=" << xmax
            << ", ncelm=" << ncelm << ") invalid arguments: xmin >= xmax"
        );
    }
    // Mark the boundary of conservation celms.
    const real_type xspace = (xmax - xmin) / ncelm;
    const size_t nx = ncelm*2+3;
    m_xcoord = array_type(std::vector<size_t>{nx});
    // Fill x-coordinates at CE boundary.
    m_xcoord[1] = xmin;
    for (size_t it=3; it<nx-2; it+=2)
    {
        m_xcoord[it] = m_xcoord[it-2] + xspace;
    }
    m_xcoord[nx-2] = xmax;
    // Fill x-coordinates at CE center.
    for (size_t it=0; it<ncelm; ++it)
    {
        m_xcoord[it*2+2] = (m_xcoord[it*2+1] + m_xcoord[it*2+3])/2;
    }
    // Fill the front and back values.
    m_xcoord[0] = m_xcoord[1] + m_xcoord[1] - m_xcoord[2];
    m_xcoord[nx-1] = m_xcoord[nx-2] + m_xcoord[nx-2] - m_xcoord[nx-3];
}

inline Celm Grid::celm(size_t ielm)
{
    return Celm(*this, ielm);
}

inline Celm Grid::celm(size_t ielm, bool odd_plane)
{
    return Celm(*this, ielm, odd_plane);
}

inline Celm Grid::celm_at(size_t ielm)
{
    const Celm elm = celm(ielm);
    if (elm.xindex() < 2 || elm.xindex() >= this->xsize()-2) {
        throw std::out_of_range(Formatter()
            << "Grid::celm_at(ielm=" << ielm << "): xindex = " << elm.xindex()
            << " outside the interval [2, " << this->xsize()-2 << ")"
        );
    }
    return elm;
}

inline Celm Grid::celm_at(size_t ielm, bool odd_plane)
{
    const Celm elm = celm(ielm, odd_plane);
    if (elm.xindex() < 2 || elm.xindex() >= this->xsize()-2) {
        throw std::out_of_range(Formatter()
            << "Grid::celm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [2, " << this->xsize()-2 << ")"
        );
    }
    return elm;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
