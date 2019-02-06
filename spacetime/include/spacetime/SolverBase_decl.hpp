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
#include "spacetime/Field_decl.hpp"

namespace spacetime
{

class Selm;

/**
 * Algorithmic definition for solution.  It holds the type information for the
 * CE and SE.
 */
template< typename ST, typename CE, typename SE >
class SolverBase
  : public std::enable_shared_from_this<ST>
{

public:

    using value_type = Field::value_type;
    using array_type = Field::array_type;
    using celm_type = CE;
    using selm_type = SE;

protected:

    class ctor_passkey {};

    template<class ... Args> static std::shared_ptr<ST> construct_impl(Args&& ... args)
    {
        return std::make_shared<ST>(std::forward<Args>(args) ..., ctor_passkey());
    }

public:

    SolverBase(std::shared_ptr<Grid> const & grid, size_t nvar, value_type time_increment, ctor_passkey const &)
      : m_field(grid, nvar, time_increment) {}

    SolverBase() = delete;
    SolverBase(SolverBase const & ) = delete;
    SolverBase(SolverBase       &&) = delete;
    SolverBase & operator=(SolverBase const & ) = delete;
    SolverBase & operator=(SolverBase       &&) = delete;

    Grid const & grid() const { return m_field.grid(); }
    Grid       & grid()       { return m_field.grid(); }

    array_type xctr(bool odd_plane) const;

    array_type const & so0() const { return m_field.so0(); }
    array_type       & so0()       { return m_field.so0(); }
    array_type const & so1() const { return m_field.so1(); }
    array_type       & so1()       { return m_field.so1(); }

    array_type get_so0(size_t iv, bool odd_plane) const;
    array_type get_so1(size_t iv, bool odd_plane) const;
    void set_so0(size_t iv, array_type const & arr, bool odd_plane);
    void set_so1(size_t iv, array_type const & arr, bool odd_plane);

    size_t nvar() const { return m_field.nvar(); }

    void set_time_increment(value_type time_increment) { m_field.set_time_increment(time_increment); }

    real_type time_increment() const { return m_field.time_increment(); }
    real_type dt() const { return m_field.dt(); }
    real_type hdt() const { return m_field.hdt(); }
    real_type qdt() const { return m_field.qdt(); }

    CE const celm(sindex_type ielm, bool odd_plane) const { return m_field.celm<CE>(ielm, odd_plane); }
    CE       celm(sindex_type ielm, bool odd_plane)       { return m_field.celm<CE>(ielm, odd_plane); }
    CE const celm_at(sindex_type ielm, bool odd_plane) const { return m_field.celm_at<CE>(ielm, odd_plane); }
    CE       celm_at(sindex_type ielm, bool odd_plane)       { return m_field.celm_at<CE>(ielm, odd_plane); }

    SE const selm(sindex_type ielm, bool odd_plane) const { return m_field.selm<SE>(ielm, odd_plane); }
    SE       selm(sindex_type ielm, bool odd_plane)       { return m_field.selm<SE>(ielm, odd_plane); }
    SE const selm_at(sindex_type ielm, bool odd_plane) const { return m_field.selm_at<SE>(ielm, odd_plane); }
    SE       selm_at(sindex_type ielm, bool odd_plane)       { return m_field.selm_at<SE>(ielm, odd_plane); }

    void march_half_so0(bool odd_plane);
    void march_half_so1(bool odd_plane);

    void treat_boundary_so0();
    void treat_boundary_so1();

    void march_full();

protected:

    Field m_field;

}; /* end class SolverBase */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
