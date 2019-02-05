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
typename CelmBase<SE>::value_type CelmBase<SE>::calc_so0(size_t iv)
{
    const auto se_xn = selm_xn();
    const auto se_xp = selm_xp();
    const value_type flux_ll = se_xn.xp(iv) + se_xn.tp(iv);
    const value_type flux_ur = se_xp.xn(iv) - se_xp.tp(iv);
    const auto se_tp = selm_tp();
    return (flux_ll + flux_ur) / se_tp.dx();
}

template< typename SE >
inline
typename CelmBase<SE>::value_type CelmBase<SE>::calc_so1(size_t iv)
{
    const auto se_xn = selm_xn();
    const auto se_xp = selm_xp();
    const value_type upp = se_xn.so0p(iv);
    const value_type upn = se_xp.so0p(iv);
    const auto se_tp = selm_tp();
    return (upn - upp) / se_tp.dx();
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
