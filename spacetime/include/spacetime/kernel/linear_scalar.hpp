#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

/**
 * Linear scalar equation.
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/Field_decl.hpp"
#include "spacetime/SolverBase_decl.hpp"

namespace spacetime
{

class LinearScalarSelm
  : public Selm
{

public:

    using base_type = Selm;
    using base_type::base_type;

    value_type xn(size_t iv) const;
    value_type xp(size_t iv) const;
    value_type tn(size_t iv) const;
    value_type tp(size_t iv) const;

    value_type so0p(size_t iv) const;

}; /* end class LinearScalarSelm */

LinearScalarSelm::value_type LinearScalarSelm::xn(size_t iv) const
{
    const value_type displacement = 0.5 * (x() + xneg()) - xctr();
    return dxneg() * (so0(iv) + displacement * so1(iv));
}

LinearScalarSelm::value_type LinearScalarSelm::xp(size_t iv) const
{
    const value_type displacement = 0.5 * (x() + xpos()) - xctr();
    return dxpos() * (so0(iv) + displacement * so1(iv));
}

LinearScalarSelm::value_type LinearScalarSelm::tn(size_t iv) const
{
    const value_type displacement = x() - xctr();
    value_type ret = so0(iv); /* f(u) */
    ret += displacement * so1(iv); /* displacement in x; f_u == 1 */
    ret += qdt() * so1(iv); /* displacement in t */
    return hdt() * ret;
}

LinearScalarSelm::value_type LinearScalarSelm::tp(size_t iv) const
{
    const value_type displacement = x() - xctr();
    value_type ret = so0(iv); /* f(u) */
    ret += displacement * so1(iv); /* displacement in x; f_u == 1 */
    ret -= qdt() * so1(iv); /* displacement in t */
    return hdt() * ret;
}

LinearScalarSelm::value_type LinearScalarSelm::so0p(size_t iv) const
{
    value_type ret = so0(iv);
    ret += (x()-xctr()) * so1(iv); /* displacement in x */
    ret -= hdt() * so1(iv); /* displacement in t */
    return ret;
}

using LinearScalarCelm = CelmBase<LinearScalarSelm>;

class LinearScalarSolver
  : public SolverBase<LinearScalarSolver, LinearScalarCelm, LinearScalarSelm>
{

public:

    using base_type = SolverBase<LinearScalarSolver, LinearScalarCelm, LinearScalarSelm>;
    using base_type::base_type;

    static std::shared_ptr<LinearScalarSolver> construct(std::shared_ptr<Grid> const & grid, value_type time_increment)
    {
        return construct_impl(grid, 1, time_increment);
    }

    void treat_so0();
    void treat_so1();

}; /* end class LinearScalarSolver */

void LinearScalarSolver::treat_so0()
{
    selm_type const selm_left_in = selm(0, true);
    selm_type       selm_left_out = selm(-1, true);
    selm_type const selm_right_in = selm(grid().ncelm()-1, true);
    selm_type       selm_right_out = selm(grid().ncelm(), true);

    selm_left_out.so0(0) = selm_right_in.so0(0);
    selm_right_out.so0(0) = selm_left_in.so0(0);
}

void LinearScalarSolver::treat_so1()
{
    selm_type const selm_left_in = selm(0, true);
    selm_type       selm_left_out = selm(-1, true);
    selm_type const selm_right_in = selm(grid().ncelm()-1, true);
    selm_type       selm_right_out = selm(grid().ncelm(), true);

    selm_left_out.so1(0) = selm_right_in.so1(0);
    selm_right_out.so1(0) = selm_left_in.so1(0);
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
