#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"

namespace spacetime
{

class Grid;
class Solution;

template< class ET >
class ElementBase
{

public:

    using base_type = ElementBase;
    using element_type = ET;

    ElementBase(real_type * xptr) : m_xptr(xptr) {}

    ET duplicate() { return *static_cast<ET *>(this); }

    Grid const & grid() const { return static_cast<ET const *>(this)->grid(); }

    real_type x() const { return *m_xptr; }
    real_type xneg() const { return *(m_xptr-1); }
    real_type xpos() const { return *(m_xptr+1); }
    real_type xctr() const { return static_cast<ET const *>(this)->xctr(); }

    ET & move(ssize_t offset)
    {
        m_xptr += offset;
        return *static_cast<ET *>(this);
    }

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

    real_type * m_xptr;

    friend Grid;
    friend Solution;

}; /* end class ElementBase */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
