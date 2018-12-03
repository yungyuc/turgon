#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/system.hpp"
#include "spacetime/Selm_decl.hpp"

namespace spacetime
{

/**
 * This class template defines the interface for flux calculation on a
 * solution element.
 */
template< typename FT, typename ST >
class FelmBase : public Selm
{

public:

    using flux_type = FT;
    using solution_type = ST;
    using value_type = typename ST::value_type;

    FelmBase(Selm && selm) : Selm(selm.field(), selm.index()) {}

}; /* end class FelmBase */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
