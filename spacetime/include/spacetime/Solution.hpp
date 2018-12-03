#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/SolutionBase_decl.hpp"
#include "spacetime/Field.hpp"

namespace spacetime
{

class Solution
  : public SolutionBase<Solution>
{

public:

    using SolutionBase<Solution>::SolutionBase;

    static std::shared_ptr<Solution> construct(std::shared_ptr<Grid> const & grid, size_t nvar, value_type time_increment)
    {
        return construct_impl(grid, nvar, time_increment);
    }

}; /* end class Solution */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
