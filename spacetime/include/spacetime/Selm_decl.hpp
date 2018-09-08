#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/Solution_decl.hpp"

namespace spacetime
{

/**
 * A solution element.
 */
class Selm
  : public ElementBase<Selm>
{

public:

    Selm(Solution & sol, size_t index)
      : base_type(sol.grid().xptr_selm(index, Grid::SelmPK()))
      , m_sol(&sol)
    {}

    Selm(Solution & sol, size_t index, bool odd_plane)
      : base_type(sol.grid().xptr_selm(index, odd_plane, Grid::SelmPK()))
      , m_sol(&sol)
    {}

    /**
     * Selm index.
     */
    index_type index() const { return (xindex() - 1) >> 1; }

    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool on_even_plane() const { return !on_odd_plane(); }
    bool on_odd_plane() const { return bool((xindex() - 1) & 1); }

    Grid const & grid() const { return sol().grid(); }
    Solution const & sol() const { return *m_sol; }

    Selm & move_at(ssize_t offset);

private:

    Solution * m_sol;

}; /* end class Selm */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
