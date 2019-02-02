#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <memory>
#include <vector>

#include "xtensor/xarray.hpp"
#include "xtensor/xfixed.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/Grid_decl.hpp"

namespace spacetime
{

class Celm;
class Selm;

/**
 * Data class for solution.  It doesn't contain type information for the CE and
 * SE.
 */
class Field
{

public:

    using value_type = Grid::value_type;
    using array_type = Grid::array_type;

    Field(std::shared_ptr<Grid> const & grid, size_t nvar, value_type time_increment);

    Field() = delete;
    Field(Field const & ) = delete;
    Field(Field       &&) = delete;
    Field & operator=(Field const & ) = delete;
    Field & operator=(Field       &&) = delete;

    Grid const & grid() const { return *m_grid; }
    Grid       & grid()       { return *m_grid; }

    array_type const & so0() const { return m_so0; }
    array_type       & so0()       { return m_so0; }
    array_type const & so1() const { return m_so1; }
    array_type       & so1()       { return m_so1; }

    value_type const & so0(size_t it, size_t iv) const { return m_so0(it, iv); }
    value_type       & so0(size_t it, size_t iv)       { return m_so0(it, iv); }
    value_type const & so1(size_t it, size_t iv) const { return m_so1(it, iv); }
    value_type       & so1(size_t it, size_t iv)       { return m_so1(it, iv); }

    size_t nvar() const { return m_so0.shape()[1]; }

    void set_time_increment(value_type time_increment);

    real_type time_increment() const { return m_time_increment; }
    real_type dt() const { return m_time_increment; }
    real_type hdt() const { return m_half_time_increment; }
    real_type qdt() const { return m_quarter_time_increment; }

    template< typename CE > CE celm(size_t ielm, bool odd_plane) { return CE(*this, ielm, odd_plane); }
    template< typename CE > CE celm_at(size_t ielm, bool odd_plane);

    template< typename SE > SE selm(size_t ielm, bool odd_plane) { return SE(*this, ielm, odd_plane); }
    template< typename SE > SE selm_at(size_t ielm, bool odd_plane);

private:

    std::shared_ptr<Grid> m_grid;

    array_type m_so0;
    array_type m_so1;

    real_type m_time_increment;
    // Cached value;
    real_type m_half_time_increment;
    real_type m_quarter_time_increment;

}; /* end class Field */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
