#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/SolverBase_decl.hpp"
#include "spacetime/Field.hpp"

namespace spacetime
{

class Solver
  : public SolverBase<Solver, Celm, Selm>
{

public:

    using base_type = SolverBase<Solver, Celm, Selm>;
    using base_type::base_type;

    Solver(Solver const &) = default;
    Solver & operator=(Solver const &) = default;

    static std::shared_ptr<Solver> construct(std::shared_ptr<Grid> const & grid, size_t nvar, value_type time_increment)
    {
        return construct_impl(grid, nvar, time_increment);
    }

    std::shared_ptr<Solver> clone(bool grid=false)
    {
        return clone_impl(grid);
    }

}; /* end class Solver */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
