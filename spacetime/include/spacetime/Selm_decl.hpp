#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/Field_decl.hpp"

namespace spacetime
{

/**
 * A solution element.
 */
class Selm
  : public ElementBase<Selm>
{

public:

    Selm(Field & field, size_t index, bool odd_plane)
      : base_type(&field, field.grid().xptr_selm(index, odd_plane, Grid::SelmPK()))
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

    value_type dxneg() const { return x()-xneg(); }
    value_type dxpos() const { return xpos()-x(); }
    value_type xctr() const { return (xneg()+xpos())/2; }

    void move_at(ssize_t offset);

    value_type const & so0(size_t iv) const { return field().so0(xindex(), iv); }
    value_type       & so0(size_t iv)       { return field().so0(xindex(), iv); }

    value_type const & so1(size_t iv) const { return field().so1(xindex(), iv); }
    value_type       & so1(size_t iv)       { return field().so1(xindex(), iv); }

    value_type xn(size_t iv) const { return 0.0; }
    value_type xp(size_t iv) const { return 0.0; }
    value_type tn(size_t iv) const { return 0.0; }
    value_type tp(size_t iv) const { return 0.0; }

    value_type so0p(size_t iv) const { return so0(iv); }

}; /* end class Selm */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
