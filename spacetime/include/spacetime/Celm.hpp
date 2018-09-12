#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/Celm_decl.hpp"

namespace spacetime
{

Celm & Celm::move_at(ssize_t offset)
{
    const ssize_t xindex = this->xindex() + offset;
    if (xindex < 2 || xindex >= grid().xsize()-2) {
        throw std::out_of_range(Formatter()
            << "Celm(xindex=" << this->xindex() << ")::move_at(offset=" << offset
            << "): xindex = " << xindex
            << " outside the interval [2, " << grid().xsize()-2 << ")"
        );
    }
    return move(offset);
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
