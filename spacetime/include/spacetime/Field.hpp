#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/Field_decl.hpp"
#include "spacetime/Celm_decl.hpp"
#include "spacetime/Selm_decl.hpp"

namespace spacetime
{

Field::Field(std::shared_ptr<Grid> const & grid, size_t nvar, Field::value_type time_increment)
  : m_grid(grid)
  , m_so0(array_type(std::vector<size_t>{grid->xsize(), nvar}))
  , m_so1(array_type(std::vector<size_t>{grid->xsize(), nvar}))
{
    set_time_increment(time_increment);
}

void Field::set_time_increment(value_type time_increment)
{
    m_time_increment = time_increment;
    m_half_time_increment = 0.5 * time_increment;
    m_quarter_time_increment = 0.25 * time_increment;
}

inline Celm Field::celm(size_t ielm)
{
    return Celm(*this, ielm);
}

inline Celm Field::celm(size_t ielm, bool odd_plane)
{
    return Celm(*this, ielm, odd_plane);
}

inline Celm Field::celm_at(size_t ielm)
{
    const Celm elm = celm(ielm);
    if (elm.xindex() < 2 || elm.xindex() >= grid().xsize()-2) {
        throw std::out_of_range(Formatter()
            << "Field::celm_at(ielm=" << ielm << "): xindex = " << elm.xindex()
            << " outside the interval [2, " << grid().xsize()-2 << ")"
        );
    }
    return elm;
}

inline Celm Field::celm_at(size_t ielm, bool odd_plane)
{
    const Celm elm = celm(ielm, odd_plane);
    if (elm.xindex() < 2 || elm.xindex() >= grid().xsize()-2) {
        throw std::out_of_range(Formatter()
            << "Field::celm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [2, " << grid().xsize()-2 << ")"
        );
    }
    return elm;
}

inline Selm Field::selm(size_t ielm)
{
    return Selm(*this, ielm);
}

inline Selm Field::selm(size_t ielm, bool odd_plane)
{
    return Selm(*this, ielm, odd_plane);
}

inline Selm Field::selm_at(size_t ielm)
{
    const Selm elm = selm(ielm);
    if (elm.xindex() < 1 || elm.xindex() >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Field::selm_at(ielm=" << ielm << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    return elm;
}

inline Selm Field::selm_at(size_t ielm, bool odd_plane)
{
    const Selm elm = selm(ielm, odd_plane);
    if (elm.xindex() < 1 || elm.xindex() >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Field::selm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    return elm;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
