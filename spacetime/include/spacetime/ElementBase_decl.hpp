#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "xtensor/xarray.hpp"

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"

namespace spacetime
{

class Grid;
class Field;

template< class ET >
class ElementBase
{

public:

    using value_type = real_type;
    using array_type = xt::xarray<value_type, xt::layout_type::row_major>;
    using base_type = ElementBase;
    using element_type = ET;

    ElementBase(Field * field, value_type * xptr) : m_field(field), m_xptr(xptr) {}

    ElementBase() = delete;

    ET duplicate() { return *static_cast<ET *>(this); }

    Grid const & grid() const;
    Field const & field() const { return *m_field; }
    Field       & field()       { return *m_field; }

    value_type time_increment() const { return field().time_increment(); }
    value_type dt() const { return field().dt(); }
    value_type hdt() const { return field().hdt(); }
    value_type qdt() const { return field().qdt(); }

    value_type x() const { return *m_xptr; }
    value_type dx() const { return xpos() - xneg(); }
    value_type xneg() const { return *(m_xptr-1); }
    value_type xpos() const { return *(m_xptr+1); }
    value_type xctr() const { return static_cast<ET const *>(this)->xctr(); }

    ET & move(ssize_t offset);
    ET & move_at(ssize_t offset) { return static_cast<ET *>(this)->move_at(offset); }

    ET & move_left() { return move(-2); }
    ET & move_right() { return move(2); }
    ET & move_neg() { return move(-1); }
    ET & move_pos() { return move(1); }

    ET & move_left_at() { return move_at(-2); }
    ET & move_right_at() { return move_at(2); }
    ET & move_neg_at() { return move_at(-1); }
    ET & move_pos_at() { return move_at(1); }

protected:

    size_t xindex() const;

    Field * m_field;
    value_type * m_xptr;

    friend Grid;
    friend Field;

}; /* end class ElementBase */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
