#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/SolutionBase_decl.hpp"
#include "spacetime/Selm_decl.hpp"

namespace spacetime
{

/**
 * A compound conservation celm.
 */
class Celm
  : public ElementBase<Celm>
{

public:

    Celm(Field & field, size_t index)
      : base_type(&field, field.grid().xptr_celm(index, Grid::CelmPK()))
    {}

    Celm(Field & field, size_t index, bool odd_plane)
      : base_type(&field, field.grid().xptr_celm(index, odd_plane, Grid::CelmPK()))
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

    value_type xctr() const { return x(); }

    Celm & move_at(ssize_t offset);

    value_type time_increment() const { return field().time_increment(); }
    value_type dt() const { return field().dt(); }
    value_type hdt() const { return field().hdt(); }
    value_type qdt() const { return field().qdt(); }

    Selm selm_xn() { return Selm(field(), index(), on_odd_plane()); }
    Selm selm_xp() { return Selm(field(), index()+1, on_odd_plane()); }
    Selm selm_tn() { return Selm(field(), index(), !on_odd_plane()); }
    Selm selm_tp() { return Selm(field(), index(), !on_odd_plane()); }

}; /* end class Celm */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
