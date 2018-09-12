#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"

namespace spacetime
{

/**
 * A compound conservation celm.
 */
class Celm
  : public ElementBase<Celm>
{

public:

    Celm(Grid & grid, size_t index)
      : base_type(grid.xptr_celm(index, Grid::CelmPK()))
      , m_grid(&grid)
    {}

    Celm(Grid & grid, size_t index, bool odd_plane)
      : base_type(grid.xptr_celm(index, odd_plane, Grid::CelmPK()))
      , m_grid(&grid)
    {}

    /**
     * Celm index.
     */
    index_type index() const { return (xindex() - 2) >> 1; }

    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool on_even_plane() const { return !on_odd_plane(); }
    bool on_odd_plane() const { return bool((xindex() - 2) & 1); }

    Grid const & grid() const { return *m_grid; }

    real_type xctr() const { return x(); }

    Celm & move_at(ssize_t offset);

private:

    Grid * m_grid;

}; /* end class Celm */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
