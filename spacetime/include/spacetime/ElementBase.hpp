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

    ElementBase(Grid & grid, real_type * xptr);

    element_type duplicate() { return *static_cast<element_type *>(this); }

    Grid const & grid() const;

    real_type x() const { return *m_xptr; }
    real_type xneg() const { return *(m_xptr-1); }
    real_type xpos() const { return *(m_xptr+1); }

protected:

    size_t xindex() const;

    Grid * m_grid;
    real_type * m_xptr;

    friend Grid;
    friend Solution;

}; /* end class ElementBase */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
