#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/Celm_decl.hpp"

namespace spacetime
{

inline
void Celm::move_at(sindex_type offset)
{
    const size_t xindex = this->xindex() + offset;
    if (xindex < 2 || xindex >= grid().xsize()-2) {
        throw std::out_of_range(Formatter()
            << "Celm(xindex=" << this->xindex() << ")::move_at(offset=" << offset
            << "): xindex = " << xindex
            << " outside the interval [2, " << grid().xsize()-2 << ")"
        );
    }
    move(offset);
}

template< typename SE >
inline
typename CelmBase<SE>::value_type CelmBase<SE>::calc_so0(size_t iv) const
{
    const SE se_xn = selm_xn();
    const SE se_xp = selm_xp();
    const value_type flux_ll = se_xn.xp(iv) + se_xn.tp(iv);
    const value_type flux_ur = se_xp.xn(iv) - se_xp.tp(iv);
    const SE se_tp = selm_tp();
    return (flux_ll + flux_ur) / se_tp.dx();
}

template< typename SE >
inline
typename CelmBase<SE>::value_type CelmBase<SE>::calc_so1(size_t iv) const
{
    // Fetch value.
    const SE se_xn = selm_xn();
    const SE se_xp = selm_xp();
    const value_type upp = se_xn.so0p(iv);
    const value_type upn = se_xp.so0p(iv);
    const SE se_tp = selm_tp();
    const value_type up = se_tp.so0(iv);
    // alpha-scheme. (alpha = 2)
    const value_type duxn = (up - upn) / se_xn.dxpos();
    const value_type duxp = (upp - up) / se_xp.dxneg();
    const value_type fan = duxn*duxn;
    const value_type fap = duxp*duxp;
    constexpr value_type tiny = std::numeric_limits<value_type>::min();
    return (fap*duxn + fan*duxp) / (fap + fan + tiny);
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
