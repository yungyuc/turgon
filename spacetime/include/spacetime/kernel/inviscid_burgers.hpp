#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

/**
 * Inviscid Burgers' equation.
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/Field_decl.hpp"
#include "spacetime/SolutionBase_decl.hpp"
#include "spacetime/FelmBase.hpp"

namespace spacetime
{

class InviscidBurgersFelm;

class InviscidBurgersSolution
  : public SolutionBase<InviscidBurgersSolution>
{

public:

    using SolutionBase<InviscidBurgersSolution>::SolutionBase;

    static std::shared_ptr<InviscidBurgersSolution> construct(std::shared_ptr<Grid> const & grid, value_type time_increment)
    {
        return construct_impl(grid, 1, time_increment);
    }

    InviscidBurgersFelm felm(Selm && self);
    InviscidBurgersFelm felm(size_t ielm);
    InviscidBurgersFelm felm(size_t ielm, bool odd_plane);
    InviscidBurgersFelm felm_at(size_t ielm);
    InviscidBurgersFelm felm_at(size_t ielm, bool odd_plane);

    void march_half_so0(bool odd_plane);
    void march_half_so1(bool odd_plane);

}; /* end class InviscidBurgersSolution */

/**
 * Flux calculator for the solution element for the inviscid Burgers equation.
 */
class InviscidBurgersFelm
  : public FelmBase<InviscidBurgersFelm, InviscidBurgersSolution>
{

public:

    using base_type = FelmBase<InviscidBurgersFelm, InviscidBurgersSolution>;
    using base_type::base_type;

    value_type xn(size_t it) const;
    value_type xp(size_t it) const;
    value_type tn(size_t it) const;
    value_type tp(size_t it) const;

    value_type so0p(size_t it) const;

}; /* end class FelmBase */

/**
 * Flux for the negative branch on the x-plane. (Flux direction in forward t.)
 */
InviscidBurgersFelm::value_type InviscidBurgersFelm::xn(size_t it) const {
    const value_type area = x() - xneg();
    const value_type displacement = 0.5 * (x() + xneg()) - xctr();
    return area * (so0(it) + displacement * so1(it));
}

/**
 * Flux for the positive branch on the x-plane. (Flux direction in forward t.)
 */
InviscidBurgersFelm::value_type InviscidBurgersFelm::xp(size_t it) const {
    const value_type area = xpos() - x();
    const value_type displacement = 0.5 * (x() + xpos()) - xctr();
    return area * (so0(it) + displacement * so1(it));
}

/**
 * Flux for the backward (behind) branch on the t-plane. (Flux direction in positive x.)
 */
InviscidBurgersFelm::value_type InviscidBurgersFelm::tn(size_t it) const {
    const value_type displacement = x() - xctr();
    const value_type u = so0(it);
    const value_type u_2 = u * u;
    value_type ret = 0.5 * u_2; /* f(u) */
    ret += displacement * u * so1(it); /* displacement in x */
    ret += qdt() * u_2 * so1(it); /* displacement in t */
    return hdt() * ret;
}

/**
 * Flux for the forward (ahead) branch on the t-plane. (Flux direction in positive x.)
 */
InviscidBurgersFelm::value_type InviscidBurgersFelm::tp(size_t it) const {
    const value_type displacement = x() - xctr();
    const value_type u = so0(it);
    const value_type u_2 = u * u;
    value_type ret = 0.5 * u_2; /* f(u) */
    ret += displacement * u * so1(it); /* displacement in x */
    ret -= qdt() * u_2 * so1(it); /* displacement in t */
    return hdt() * ret;
}

/**
 * Approximated value of the solution variable at the t+ tip of the solution element.
 */
InviscidBurgersFelm::value_type InviscidBurgersFelm::so0p(size_t it) const {
    value_type ret = so0(it);
    ret += (x()-xctr()) * so1(it); /* displacement in x */
    ret -= hdt() * so1(it); /* displacement in t */
    return ret * so0(it);
}

inline InviscidBurgersFelm InviscidBurgersSolution::felm(Selm && selm) { return InviscidBurgersFelm(std::move(selm)); }
inline InviscidBurgersFelm InviscidBurgersSolution::felm(size_t ielm) { return InviscidBurgersFelm(selm(ielm)); }
inline InviscidBurgersFelm InviscidBurgersSolution::felm(size_t ielm, bool odd_plane) { return InviscidBurgersFelm(selm(ielm, odd_plane)); }
inline InviscidBurgersFelm InviscidBurgersSolution::felm_at(size_t ielm) { return InviscidBurgersFelm(selm_at(ielm)); }
inline InviscidBurgersFelm InviscidBurgersSolution::felm_at(size_t ielm, bool odd_plane) { return InviscidBurgersFelm(selm_at(ielm, odd_plane)); }

void InviscidBurgersSolution::march_half_so0(bool odd_plane)
{
    size_t ncelm = this->grid().ncelm();
    if (odd_plane) { ncelm -= 1;}
    for (size_t ic=0; ic<ncelm; ++ic)
    {
        auto ce = this->celm(ic, odd_plane);
        auto fe_xn = felm(ce.selm_xn());
        auto fe_xp = felm(ce.selm_xp());
        value_type flux_ll = fe_xn.xp(0) + fe_xn.tp(0);
        value_type flux_ur = fe_xp.xn(0) - fe_xp.tp(0);
        auto se = ce.selm_tp();
        se.so0(0) = (flux_ll + flux_ur) / se.dx();
    }
}

void InviscidBurgersSolution::march_half_so1(bool odd_plane)
{
    size_t ncelm = this->grid().ncelm();
    if (odd_plane) { ncelm -= 1;}
    for (size_t ic=0; ic<ncelm; ++ic)
    {
        auto ce = this->celm(ic, odd_plane);
        auto fe_xn = felm(ce.selm_xn());
        auto fe_xp = felm(ce.selm_xp());
        value_type upp = fe_xn.so0p(0);
        value_type upn = fe_xp.so0p(0);
        auto se = ce.selm_tp();
        se.so1(0) = (upn - upp) / se.dx();
    }
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
