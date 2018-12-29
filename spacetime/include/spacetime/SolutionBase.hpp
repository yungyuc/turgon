#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/SolutionBase_decl.hpp"

namespace spacetime
{

template< typename ST > inline
Celm SolutionBase<ST>::celm(size_t ielm) { return m_field.celm(ielm); }
template< typename ST > inline
Celm SolutionBase<ST>::celm(size_t ielm, bool odd_plane) { return m_field.celm(ielm, odd_plane); }
template< typename ST > inline
Celm SolutionBase<ST>::celm_at(size_t ielm) { return m_field.celm_at(ielm); }
template< typename ST > inline
Celm SolutionBase<ST>::celm_at(size_t ielm, bool odd_plane) { return m_field.celm_at(ielm, odd_plane); }

template< typename ST > inline
Selm SolutionBase<ST>::selm(size_t ielm) { return m_field.selm(ielm); }
template< typename ST > inline
Selm SolutionBase<ST>::selm(size_t ielm, bool odd_plane) { return m_field.selm(ielm, odd_plane); }
template< typename ST > inline
Selm SolutionBase<ST>::selm_at(size_t ielm) { return m_field.selm_at(ielm); }
template< typename ST > inline
Selm SolutionBase<ST>::selm_at(size_t ielm, bool odd_plane) { return m_field.selm_at(ielm, odd_plane); }

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
