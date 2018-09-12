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
inline size_t ElementBase<ET>::xindex() const { return m_xptr - grid().xptr(); }

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
