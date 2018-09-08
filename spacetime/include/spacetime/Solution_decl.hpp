#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <memory>
#include <vector>

#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/Grid_decl.hpp"

namespace spacetime
{

class Selm;

class Solution
  : public std::enable_shared_from_this<Solution>
{

public:

    using value_type = Grid::value_type;
    using array_type = Grid::array_type;

private:

    class ctor_passkey {};

public:

    template<class ... Args>
    static std::shared_ptr<Solution> construct(Args&& ... args)
    {
        return std::make_shared<Solution>(std::forward<Args>(args) ..., ctor_passkey());
    }

    Solution(std::shared_ptr<Grid> const & grid, size_t nvar, ctor_passkey const &)
      : m_grid(grid)
      , m_vars(array_type(std::vector<size_t>{grid->xsize(), nvar}))
    {}

    Solution() = delete;
    Solution(Solution const & ) = delete;
    Solution(Solution       &&) = delete;
    Solution & operator=(Solution const & ) = delete;
    Solution & operator=(Solution       &&) = delete;

    Grid const & grid() const { return *m_grid; }
    Grid       & grid()       { return *m_grid; }

    size_t   nvar() const { return m_vars.shape()[1]; }

    Selm selm(size_t ielm);
    Selm selm(size_t ielm, bool odd_plane);
    Selm selm_at(size_t ielm);
    Selm selm_at(size_t ielm, bool odd_plane);

private:

    std::shared_ptr<Grid> m_grid;
    array_type m_vars;

}; /* end class Solution */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
