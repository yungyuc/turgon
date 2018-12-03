#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid.hpp"

namespace spacetime
{

template< class ET >
Grid const & ElementBase<ET>::grid() const { return m_field->grid(); }

template< class ET >
size_t ElementBase<ET>::xindex() const { return m_xptr - grid().xptr(); }

template< class ET >
ET & ElementBase<ET>::move(ssize_t offset)
{
    m_xptr += offset;
    return *static_cast<ET *>(this);
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
