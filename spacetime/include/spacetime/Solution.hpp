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
#include "spacetime/ElementBase.hpp"
#include "spacetime/Grid.hpp"

namespace spacetime
{

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

    Solution(std::shared_ptr<Grid> const & grid, size_t nvar, ctor_passkey const &);

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

Solution::Solution(std::shared_ptr<Grid> const & grid, size_t nvar, ctor_passkey const &)
  : m_grid(grid)
{
    m_vars = array_type(std::vector<size_t>{grid->xsize(), nvar});
}

/**
 * A solution element.
 */
class Selm
  : public ElementBase<Selm>
{

public:

    Selm(Grid & grid, size_t index)
      : base_type(
            grid
          , grid.xptr_selm(index, Grid::SelmPK())
        )
    {}

    Selm(Grid & grid, size_t index, bool odd_plane)
      : base_type(
            grid
          , grid.xptr_selm(index, odd_plane, Grid::SelmPK())
        )
    {}

    /**
     * Selm index.
     */
    index_type index() const { return (xindex() - 1) >> 1; }

    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool on_even_plane() const { return !bool((xindex() - 1) & 1); }

    Selm & move(ssize_t offset)
    {
        m_xptr += offset;
        return *static_cast<Selm *>(this);
    }

    Selm & move_at(ssize_t offset);

    Selm & move_left() { return move(-2); }
    Selm & move_right() { return move(2); }
    Selm & move_neg() { return move(-1); }
    Selm & move_pos() { return move(1); }

    Selm & move_left_at() { return move_at(-2); }
    Selm & move_right_at() { return move_at(2); }
    Selm & move_neg_at() { return move_at(-1); }
    Selm & move_pos_at() { return move_at(1); }

}; /* end class Selm */

Selm & Selm::move_at(ssize_t offset)
{
    const ssize_t xindex = this->xindex() + offset;
    if (xindex < 1 || xindex >= m_grid->xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Selm(xindex=" << this->xindex() << ")::move_at(offset=" << offset
            << "): xindex = " << xindex
            << " outside the interval [1, " << m_grid->xsize()-1 << ")"
        );
    }
    return move(offset);
}

inline Selm Solution::selm(size_t ielm)
{
    return Selm(grid(), ielm);
}

inline Selm Solution::selm(size_t ielm, bool odd_plane)
{
    return Selm(grid(), ielm, odd_plane);
}

inline Selm Solution::selm_at(size_t ielm)
{
    const Selm elm = selm(ielm);
    if (elm.xindex() < 1 || elm.xindex() >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Grid::selm_at(ielm=" << ielm << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    return elm;
}

inline Selm Solution::selm_at(size_t ielm, bool odd_plane)
{
    const Selm elm = selm(ielm, odd_plane);
    if (elm.xindex() < 1 || elm.xindex() >= grid().xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Grid::selm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << grid().xsize()-1 << ")"
        );
    }
    return elm;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
